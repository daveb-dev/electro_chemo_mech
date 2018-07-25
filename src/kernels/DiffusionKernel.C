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
 _deformation_gradient(getMaterialPropertyByName<RankTwoTensor>("deformation_gradient")),
_stress(getMaterialPropertyByName<RankTwoTensor>("stress"))
{
}

Real
DiffusionKernel::computeQpResidual()
{
    RankTwoTensor I(RankTwoTensor::initIdentity);
    RankTwoTensor conc_deformation_gradient = (1.0 +_lattice_misfit[_qp]*_u[_qp]/_molar_volume[_qp])*I;   
    RankTwoTensor total_deformation_gradient = _deformation_gradient*conc_deformation_gradient;
    RankTwoTensor C = total_deformation_gradient.transpose()*total_deformation_gradient;
    RankTwoTensor Cinv = C.inverse();
    RankTwoTensor 
}