/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DiffusionKernel.C
 * Author: srinath
 * 
 * Created on July 25, 2018, 10:48 AM
 */

#include "DiffusionKernel.h"

registerMooseObject("electro_chemo_mechApp", DiffusionKernel);

template <>
InputParameters
validParams<DiffusionKernel>()
{
  InputParameters params = validParams<Diffusion>();
  return params;
}

DiffusionKernel::DiffusionKernel(const InputParameters& parameters) 
: Diffusion(parameters),
_mobility(getMaterialPropertyByName<Real>("mobility")),
_lattice_misfit(getMaterialPropertyByName<Real>("lattice_misfit")),
_molar_volume(getMaterialPropertyByName<Real>("molar_volume")),
_max_concentration(getMaterialPropertyByName<Real>("max_concentration")),
 _deformation_gradient(getMaterialPropertyByName<RankTwoTensor>("deformation_gradient")),
 _deformation_gradient_diffusion(getMaterialProperty<RankTwoTensor>("deformation_gradient_diffusion")),
_stress(getMaterialPropertyByName<RankTwoTensor>("stress"))
{
}

Real
DiffusionKernel::computeQpResidual()
{
    Real residual = 0.0;
    Real temperature = 298.0;
    Real gas_constant = 8.314;
    RankTwoTensor C_e = _deformation_gradient[_qp].transpose()*_deformation_gradient[_qp];
    RankTwoTensor C = _deformation_gradient_diffusion[_qp].transpose()*C_e*_deformation_gradient_diffusion[_qp];
    RankTwoTensor Cinv = C.inverse();
    Real temp1 = _max_concentration[_qp]/(_max_concentration[_qp] - _u[_qp]);
    residual =  temp1*Cinv*_grad_u[_qp]*_grad_test[_i][_qp];
    
    RankTwoTensor Ctemp = _lattice_misfit[_qp]*_u[_qp]/(gas_constant*temperature)*Cinv;
    RealGradient FetP; 
    return residual;
}