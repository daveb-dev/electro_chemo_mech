/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ChemoDiffusion.C
 * Author: srinath
 * 
 * Created on August 21, 2018, 8:06 AM
 */

#include "ChemoDiffusion.h"

registerMooseObject("electro_chemo_mechApp",ChemoDiffusion);

template <>
InputParameters
validParams<ChemoDiffusion>()
{
    InputParameters params = validParams<Diffusion>();
    params.addClassDescription(
            "Computes residual/Jacobian for TBD");
    params.addCoupledVar("stress_based_chemical_potential", 
                          "Name of the variable for the stress_based_chemical_potential");
    params.addParam<MaterialPropertyName>("diffusion_coefficient",
                                           "mobility",
                                          "Property name for diffusivity/mobility");
    params.addParam<MaterialPropertyName>("diffusion_coefficient_dC",
                                          "mobility_dC"
                                          "Property name for derivative of diffusivity with respect"
                                          "to concentration ");
    params.addParam<MaterialPropertyName>("activity_coefficient",
                                           "activity_coefficient",
                                            "Activity Coefficient");
    params.addParam<MaterialPropertyName>("activity_coefficient_dC",
                                          "activity_coefficient_dC",
                                           "Variation of activity coefficient with concentration");
    params.set<bool>("use_displaced_mesh") = false;
    return params;
}

ChemoDiffusion::ChemoDiffusion(const InputParameters & parameters)
        : Diffusion(parameters),
        _mu_coupled(isCoupled("stress_based_chemical_potential")),
        _mu_var(_mu_coupled ? coupled("stress_based_chemical_potential") : 0),
        _diffusion_coefficient(getMaterialProperty<Real>("diffusion_coefficient")),
        _diffusion_coefficient_dC(hasMaterialProperty<Real>("diffusion_coefficient_dC")
                                  ? &getMaterialProperty<Real>("diffusion_coefficient_dC")
                                  : NULL),
        _activity_coefficient(getMaterialProperty<Real>("activity_coefficient")),
        _activity_coefficient_dC(hasMaterialProperty<Real>("activity_coefficient_dC")
                                  ? &getMaterialProperty<Real>("activity_coefficient_dC")
                                  : NULL)

{
    
}

Real
ChemoDiffusion::computeQpResidual()
{
    Real residual = _diffusion_coefficient[_qp]*Diffusion::computeQpResidual();
//    if (_activity_coefficient_dC) {
//        
//        Real dgamma = (*_activity_coefficient_dC)[_qp];
//        Real R = _diffusion_coefficient[_qp] * (_u[_qp]/_activity_coefficient[_qp]) * 
//                dgamma * _grad_test[_i][_qp] ; 
//        residual += R;
//    }
    return residual;
}

Real 
ChemoDiffusion::computeQpJacobian()
{
  Real jac = _diffusion_coefficient[_qp] * Diffusion::computeQpJacobian();
  if (_diffusion_coefficient_dC) 
  {
    jac += (*_diffusion_coefficient_dC)[_qp] * _phi[_j][_qp] * Diffusion::computeQpResidual();
  }
  if (_activity_coefficient_dC)
  {
      Real gamma = _activity_coefficient[_qp];
      Real dgamma = (*_activity_coefficient_dC)[_qp];
      Real c = _u[_qp];
      
      jac += _diffusion_coefficient[_qp]*_u[_qp]/gamma * dgamma
              *_grad_phi[_j][_qp]*_grad_test[_i][_qp];
      jac += dgamma * (1.0 - c/gamma * dgamma)/gamma 
              * _phi[_j][_qp]*_grad_u[_qp]*_grad_test[_i][_qp];
  }
  
  return jac;
}

Real
ChemoDiffusion::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _mu_var)   
      return _diffusion_coefficient[_qp] * _grad_phi[_j][_qp] * _grad_test[_i][_qp];
  else
      return computeQpJacobian();
}