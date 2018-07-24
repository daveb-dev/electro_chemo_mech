/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Bucci2016.cpp
 * Author: srinath
 * 
 * Created on July 24, 2018, 9:48 AM
 */

#include "Bucci2016.h"

// MOOSE includes
#include "ElasticityTensorTools.h"
#include "Material.h"
#include "MooseMesh.h"
#include "MooseVariable.h"
#include "SystemBase.h"

#include "libmesh/quadrature.h"


registerMooseObject("electro_chemo_mechApp", Bucci2016);


template <>
InputParameters
validParams<Bucci2016>()
{
    InputParameters params = validParams<ALEKernel>();
    params.addClassDescription("Stress Divergence in Total Lagrangian formulation in Cartesian coordinates");
    params.addRequiredParam<unsigned int>("component",
                                        "An integer corresponding to the direction "
                                        "the variable this kernel acts in. (0 for x, "
                                        "1 for y, 2 for z)");
    params.addRequiredCoupledVar("displacements",
                               "The string of displacements suitable for the problem statement");
    params.addCoupledVar("concentration", 
            "The name of the concentration variable used in a different kernel");
    params.addParam<bool>("volumetric_locking_correction",
                        false,
                        "Set to false to turn off volumetric locking correction");
    MooseEnum out_of_plane_direction("x y z", "z");
    params.addParam<MooseEnum>(
      "out_of_plane_direction",
      out_of_plane_direction,
      "The direction of the out_of_plane_strain variable used in the WeakPlaneStress kernel.");
    params.addParam<std::string>("base_name", "Material property base name");
    params.set<bool>("use_displaced_mesh") = true;

    return params;
}

Bucci2016::Bucci2016(const InputParameters& parameters)
    : ALEKernel(parameters),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _stress(getMaterialPropertyByName<RankTwoTensor>(_base_name + "stress")),
    _Jacobian_mult(getMaterialPropertyByName<RankFourTensor>(_base_name + "Jacobian_mult")),
    _deformation_gradient(getMaterialPropertyByName<RankTwoTensor>(_base_name + "deformation_gradient")),
    _component(getParam<unsigned int>("component")),
    _ndisp(coupledComponents("displacements")),
    _disp_var(_ndisp),
    _conc_coupled(isCoupled("concentration")),
    _conc_var(_conc_coupled ? coupled("concentration") :0),
    _out_of_plane_direction(getParam<MooseEnum>("out_of_plane_direction"))        
    
{
  for (unsigned int i = 0; i < _ndisp; ++i)
    _disp_var[i] = coupled("displacements", i); 
}



void
Bucci2016::initialSetup()
{
  if (getBlockCoordSystem() != Moose::COORD_XYZ)
    mooseError(
        "The coordinate system in the Problem block must be set to XYZ for cartesian geometries.");
}

void
Bucci2016::computeResidual()
{
    DenseVector<Number> & re = _assembly.residualBlock(_var.number());
  _local_re.resize(re.size());
  _local_re.zero();

  
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
Bucci2016::computeQpResidual()
{
    RankTwoTensor Finv = _deformation_gradient[_qp].inverse();
    Real J = _deformation_gradient[_qp].det();
    RankTwoTensor FinvT = Finv.transpose();
    //Real mu = mu0 - lambda0*logJ;
    
    RankTwoTensor Pk1 = J*_stress[_qp]*FinvT;
    
    Real residual = Pk1.row(_component)*_grad_test[_i][_qp];
    return residual;
}

void
Bucci2016::computeJacobian()
{
    Kernel::computeJacobian();
}

void
Bucci2016::computeOffDiagJacobian(MooseVariableFEBase & jvar)
{
    Kernel::computeOffDiagJacobian(jvar);
}

Real
Bucci2016::computeQpJacobian()
{

  Real jacobian = 0.0;
  jacobian += ElasticityTensorTools::elasticJacobian(_Jacobian_mult[_qp], 
                                                     _component,
                                                     _component, 
                                                     _grad_test[_i][_qp], 
                                                     _grad_phi[_j][_qp]);
  return jacobian;
}

Real
Bucci2016::computeQpOffDiagJacobian(unsigned int jvar)
{
  for (unsigned int coupled_component = 0; coupled_component < _ndisp; ++coupled_component)
    if (jvar == _disp_var[coupled_component])
    {
        
      if (_out_of_plane_direction != 2)
      {
        if (coupled_component == _out_of_plane_direction)
          continue;
      }  
      Real jacobian = 0.0;

      jacobian += ElasticityTensorTools::elasticJacobian(_Jacobian_mult[_qp],
                                                         _component,
                                                         coupled_component,
                                                         _grad_test[_i][_qp],
                                                         _grad_phi[_j][_qp]);
      return jacobian;
    }

  return 0.0;
}



