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
    InputParameters params = validParams<StressDivergenceTensors>();
    params.addClassDescription("Stress Divergence in Total Lagrangian formulation in Cartesian coordinates");
    params.addParam<bool>(
      "use_finite_deform_jacobian", true, "Finite strain jacobian with Geometric term");
     return params;
}

StressDivergenceConcentrationTensor::StressDivergenceConcentrationTensor(const InputParameters & parameters) 
    : StressDivergenceTensors(parameters),
    _deformation_gradient(getMaterialProperty<RankTwoTensor>(_base_name + "deformation_gradient"))
{
   
}

void
StressDivergenceConcentrationTensor::computeResidual()
{
    StressDivergenceTensors::computeResidual();
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
      StressDivergenceTensors::computeAverageGradientTest();
      StressDivergenceTensors::computeAverageGradientPhi();
    }
    Kernel::computeJacobian();
}

void
StressDivergenceConcentrationTensor::computeOffDiagJacobian(MooseVariableFEBase & jvar)
{
  if (_volumetric_locking_correction)
    {
      StressDivergenceTensors::computeAverageGradientTest();
      StressDivergenceTensors::computeAverageGradientPhi();
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
      if (_out_of_plane_direction != 2)
      {
        if (coupled_component == _out_of_plane_direction)
          continue;
      }

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

  // off-diagonal Jacobian with respect to a coupled out_of_plane_strain variable
  if (_out_of_plane_strain_coupled && jvar == _out_of_plane_strain_var)
    return _Jacobian_mult[_qp](
               _component, _component, _out_of_plane_direction, _out_of_plane_direction) *
           _grad_test[_i][_qp](_component) * _phi[_j][_qp];

  // off-diagonal Jacobian with respect to a coupled temperature variable
  if (_temp_coupled && jvar == _temp_var)
    return -((_Jacobian_mult[_qp] * (*_deigenstrain_dT)[_qp]) *
             _grad_test[_i][_qp])(_component)*_phi[_j][_qp];

  return 0.0;
}