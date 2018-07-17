/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   StressDivergenceConcentrationTensor.C
 * Author: srinath
 * 
 * Created on July 9, 2018, 1:51 PM
 */

#include "StressDivergenceConcentrationTensor.h"


// MOOSE includes
#include "ElasticityTensorTools.h"
#include "Material.h"
#include "MooseMesh.h"
#include "MooseVariable.h"
#include "SystemBase.h"

#include "libmesh/quadrature.h"


registerMooseObject("electro_chemo_mechApp", StressDivergenceConcentrationTensor);


template <>
InputParameters
validParams<StressDivergenceConcentrationTensor>()
{
    InputParameters params = validParams<ALEKernel>();
    params.addClassDescription("Stress Divergence in Total Lagrangian formulation in Cartesian coordinates");
    params.addRequiredParam<unsigned int>("component",
                                        "An integer corresponding to the direction "
                                        "the variable this kernel acts in. (0 for x, "
                                        "1 for y, 2 for z)");
    params.addRequiredCoupledVar("displacements",
                               "The string of displacements suitable for the problem statement");
    params.addParam<std::string>("base_name", "Material property base name");
    params.addParam<bool>("volumetric_locking_correction",
                        false,
                        "Set to false to turn off volumetric locking correction");

    return params;
}

StressDivergenceConcentrationTensor::StressDivergenceConcentrationTensor(const InputParameters& parameters)
    : ALEKernel(parameters),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _stress(getMaterialPropertyByName<RankTwoTensor>(_base_name + "stress")),
    _Jacobian_mult(getMaterialPropertyByName<RankFourTensor>(_base_name + "Jacobian_mult")),
    _deformation_gradient(getMaterialPropertyByName<RankTwoTensor>(_base_name + "deformation_gradient")),
    _component(getParam<unsigned int>("component")),
    _ndisp(coupledComponents("displacements")),
    _disp_var(_ndisp),
    _avg_grad_test(_test.size(), std::vector<Real>(3, 0.0)),
    _avg_grad_phi(_phi.size(), std::vector<Real>(3, 0.0)),
    _volumetric_locking_correction(getParam<bool>("volumetric_locking_correction"))
    
{
  for (unsigned int i = 0; i < _ndisp; ++i)
    _disp_var[i] = coupled("displacements", i); 
    // Error if volumetic locking correction is turned on for 1D problems
  if (_ndisp == 1 && _volumetric_locking_correction)
    mooseError("Volumetric locking correction should be set to false for 1-D problems.");

}



void
StressDivergenceConcentrationTensor::initialSetup()
{
  if (getBlockCoordSystem() != Moose::COORD_XYZ)
    mooseError(
        "The coordinate system in the Problem block must be set to XYZ for cartesian geometries.");
}

void
StressDivergenceConcentrationTensor::computeResidual()
{
    DenseVector<Number> & re = _assembly.residualBlock(_var.number());
  _local_re.resize(re.size());
  _local_re.zero();

  if (_volumetric_locking_correction)
    computeAverageGradientTest();

  precalculateResidual();
  for (_i = 0; _i < _test.size(); ++_i)
    for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
      _local_re(_i) += _JxW[_qp] * _coord[_qp] * computeQpResidual();

  re += _local_re;

  if (_has_save_in)
  {
    Threads::spin_mutex::scoped_lock lock(Threads::spin_mtx);
    for (const auto & var : _save_in)
      var->sys().solution().add_vector(_local_re, var->dofIndices());
  }
}

Real
StressDivergenceConcentrationTensor::computeQpResidual()
{
    RankTwoTensor Finv = _deformation_gradient[_qp].inverse();
    Real J = _deformation_gradient[_qp].det();
    RankTwoTensor pk2_stress = J*(Finv*_stress[_qp]*Finv.transpose());
    Real residual = pk2_stress.row(_component) * _grad_test[_i][_qp];
  // volumetric locking correction
    if (_volumetric_locking_correction)
        residual += pk2_stress.trace() / 3.0 *
                (_avg_grad_test[_i][_component] - _grad_test[_i][_qp](_component));

    return residual;
}

void
StressDivergenceConcentrationTensor::computeJacobian()
{
  if (_volumetric_locking_correction)
    {
      computeAverageGradientTest();
      computeAverageGradientPhi();
    }
    Kernel::computeJacobian();
}

void
StressDivergenceConcentrationTensor::computeOffDiagJacobian(MooseVariableFEBase & jvar)
{
  if (_volumetric_locking_correction)
    {
      computeAverageGradientTest();
      computeAverageGradientPhi();
    }
    Kernel::computeOffDiagJacobian(jvar);
}

Real
StressDivergenceConcentrationTensor::computeQpJacobian()
{
  Real sum_C3x3 = _Jacobian_mult[_qp].sum3x3();
  RealGradient sum_C3x1 = _Jacobian_mult[_qp].sum3x1();

  Real jacobian = 0.0;
  // B^T_i * C * B_j
  jacobian += ElasticityTensorTools::elasticJacobian(
      _Jacobian_mult[_qp], _component, _component, _grad_test[_i][_qp], _grad_phi[_j][_qp]);

  if (_volumetric_locking_correction)
  {
    // jacobian = Bbar^T_i * C * Bbar_j where Bbar = B + Bvol
    // jacobian = B^T_i * C * B_j + Bvol^T_i * C * Bvol_j +  Bvol^T_i * C * B_j + B^T_i * C * Bvol_j

    // Bvol^T_i * C * Bvol_j
    jacobian += sum_C3x3 * (_avg_grad_test[_i][_component] - _grad_test[_i][_qp](_component)) *
                (_avg_grad_phi[_j][_component] - _grad_phi[_j][_qp](_component)) / 9.0;

    // B^T_i * C * Bvol_j
    jacobian += sum_C3x1(_component) * _grad_test[_i][_qp](_component) *
                (_avg_grad_phi[_j][_component] - _grad_phi[_j][_qp](_component)) / 3.0;

    // Bvol^T_i * C * B_j
    RankTwoTensor phi;
    if (_component == 0)
    {
      phi(0, 0) = _grad_phi[_j][_qp](0);
      phi(0, 1) = phi(1, 0) = _grad_phi[_j][_qp](1);
      phi(0, 2) = phi(2, 0) = _grad_phi[_j][_qp](2);
    }
    else if (_component == 1)
    {
      phi(1, 1) = _grad_phi[_j][_qp](1);
      phi(0, 1) = phi(1, 0) = _grad_phi[_j][_qp](0);
      phi(1, 2) = phi(2, 1) = _grad_phi[_j][_qp](2);
    }
    else if (_component == 2)
    {
      phi(2, 2) = _grad_phi[_j][_qp](2);
      phi(0, 2) = phi(2, 0) = _grad_phi[_j][_qp](0);
      phi(1, 2) = phi(2, 1) = _grad_phi[_j][_qp](1);
    }

    jacobian += (_Jacobian_mult[_qp] * phi).trace() *
                (_avg_grad_test[_i][_component] - _grad_test[_i][_qp](_component)) / 3.0;
  }
  RankTwoTensor Finv = _deformation_gradient[_qp].inverse();
  Real J = _deformation_gradient[_qp].det();
  RankTwoTensor pk2_stress = J*(Finv*_stress[_qp]*Finv.transpose());
  Real jac_geom = pk2_stress.row(_component) * _grad_phi[_j][_qp];
  jac_geom *= _grad_test[_i][_qp](_component);
  jacobian -= jac_geom;
  return jacobian;
}

Real
StressDivergenceConcentrationTensor::computeQpOffDiagJacobian(unsigned int jvar)
{
  // off-diagonal Jacobian with respect to a coupled displacement component
  for (unsigned int coupled_component = 0; coupled_component < _ndisp; ++coupled_component)
    if (jvar == _disp_var[coupled_component])
    {
      const Real sum_C3x3 = _Jacobian_mult[_qp].sum3x3();
      const RealGradient sum_C3x1 = _Jacobian_mult[_qp].sum3x1();
      Real jacobian = 0.0;

      // B^T_i * C * B_j
      jacobian += ElasticityTensorTools::elasticJacobian(_Jacobian_mult[_qp],
                                                         _component,
                                                         coupled_component,
                                                         _grad_test[_i][_qp],
                                                         _grad_phi[_j][_qp]);

      if (_volumetric_locking_correction)
      {
        // jacobian = Bbar^T_i * C * Bbar_j where Bbar = B + Bvol
        // jacobian = B^T_i * C * B_j + Bvol^T_i * C * Bvol_j +  Bvol^T_i * C * B_j + B^T_i * C *
        // Bvol_j

        // Bvol^T_i * C * Bvol_j
        jacobian += sum_C3x3 * (_avg_grad_test[_i][_component] - _grad_test[_i][_qp](_component)) *
                    (_avg_grad_phi[_j][coupled_component] - _grad_phi[_j][_qp](coupled_component)) /
                    9.0;

        // B^T_i * C * Bvol_j
        jacobian += sum_C3x1(_component) * _grad_test[_i][_qp](_component) *
                    (_avg_grad_phi[_j][coupled_component] - _grad_phi[_j][_qp](coupled_component)) /
                    3.0;

        // Bvol^T_i * C * B_i
        RankTwoTensor phi;
        for (unsigned int i = 0; i < 3; ++i)
          phi(coupled_component, i) = _grad_phi[_j][_qp](i);

        jacobian += (_Jacobian_mult[_qp] * phi).trace() *
                    (_avg_grad_test[_i][_component] - _grad_test[_i][_qp](_component)) / 3.0;
      }
      RankTwoTensor Finv = _deformation_gradient[_qp].inverse();
      Real J = _deformation_gradient[_qp].det();
      RankTwoTensor pk2_stress = J*(Finv*_stress[_qp]*Finv.transpose());
      Real jac_geom = pk2_stress.row(_component) * _grad_phi[_j][_qp];
      jac_geom *= _grad_test[_i][_qp](coupled_component);
      jacobian -= jac_geom;
      return jacobian;
    }

  return 0.0;
}

void
StressDivergenceConcentrationTensor::computeAverageGradientTest()
{
  // Calculate volume averaged value of shape function derivative
  _avg_grad_test.resize(_test.size());
  for (_i = 0; _i < _test.size(); ++_i)
  {
    _avg_grad_test[_i].resize(3);
    _avg_grad_test[_i][_component] = 0.0;
    for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
      _avg_grad_test[_i][_component] += _grad_test[_i][_qp](_component) * _JxW[_qp] * _coord[_qp];

    _avg_grad_test[_i][_component] /= _current_elem_volume;
  }
}

void
StressDivergenceConcentrationTensor::computeAverageGradientPhi()
{
  // Calculate volume average derivatives for phi
  _avg_grad_phi.resize(_phi.size());
  for (_i = 0; _i < _phi.size(); ++_i)
  {
    _avg_grad_phi[_i].resize(3);
    for (unsigned int component = 0; component < _mesh.dimension(); ++component)
    {
      _avg_grad_phi[_i][component] = 0.0;
      for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
        _avg_grad_phi[_i][component] += _grad_phi[_i][_qp](component) * _JxW[_qp] * _coord[_qp];

      _avg_grad_phi[_i][component] /= _current_elem_volume;
    }
  }
}