/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ChemoChemoDiffusionFlux.h
 * Author: srinath
 *
 * Created on September 18, 2018, 8:33 AM
 */

#ifndef CHEMODIFFUSIONFLUX_H
#define CHEMODIFFUSIONFLUX_H


#include "AuxKernel.h"

class ChemoDiffusionFluxAux;

template <>
InputParameters validParams<ChemoDiffusionFluxAux>();

/**
 * Auxiliary kernel responsible for computing the components of the flux vector
 * in diffusion problems
 */
class ChemoDiffusionFluxAux : public AuxKernel
{
public:
  ChemoDiffusionFluxAux(const InputParameters & parameters);

protected:
  virtual Real computeValue();
  /// Will hold 0, 1, or 2 corresponding to x, y, or z.
  int _component;

  
  /// Holds the gradient of the concentration at the current quadrature points
  const VariableValue & _u;

  /// Holds the gradient of the concentration at the current quadrature points
  const VariableGradient & _grad_u;

    /// Holds the gradient of the mechanical part of the chemical potential at the current quadrature points
  const VariableGradient & _grad_mu;

  
  /// Holds the diffusivity from the material system
  const MaterialProperty<Real> & _diffusion_coef;
  
  /// Holds the diffusivity from the material system
  const MaterialProperty<Real> & _mobility;
  
};



#endif /* CHEMODIFFUSIONFLUX_H */

