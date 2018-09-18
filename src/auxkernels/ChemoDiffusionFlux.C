/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ChemoChemoDiffusionFlux.C
 * Author: srinath
 * 
 * Created on September 18, 2018, 8:33 AM
 */

#include "ChemoDiffusionFlux.h"

registerMooseObject("electro_chemo_mechApp", ChemoDiffusionFluxAux);

template <>
InputParameters
validParams<ChemoDiffusionFluxAux>()
{
  InputParameters params = validParams<AuxKernel>();
  MooseEnum component("x y z");
  params.addClassDescription("Compute components of flux vector for diffusion problems "
                             "$(\\vv{J} = -D \\nabla C)$.");
  params.addRequiredParam<MooseEnum>("component", component, "The desired component of flux.");
  params.addRequiredCoupledVar("diffusion_variable", "The name of the variable");
  params.addRequiredCoupledVar("chemical_potential", "The name of the chemical potential variable");
  params.addRequiredParam<MaterialPropertyName>(
      "diffusivity",
      "The name of the diffusivity material property that will be used in the flux computation.");
  return params;
}

ChemoDiffusionFluxAux::ChemoDiffusionFluxAux(const InputParameters & parameters)
  : AuxKernel(parameters),
    _component(getParam<MooseEnum>("component")),
    _grad_u(coupledGradient("diffusion_variable")),
    _grad_mu(coupledGradient("chemical_potential")),
    _diffusion_coef(getMaterialProperty<Real>("diffusivity")),
    _mobility(getMaterialProperty<Real>("mobility"))
{
}

Real
ChemoDiffusionFluxAux::computeValue()
{
  return -(_diffusion_coef[_qp] * _grad_u[_qp](_component) + _mobility[_qp]*_grad_mu[_qp](_component));
}
