/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Compute2DTlFiniteStrain.C
 * Author: srinath
 * 
 * Created on July 17, 2018, 10:35 AM
 */

#include "Compute2DTlFiniteStrain.h"

#include "libmesh/quadrature.h"

registerMooseObject("electro_chemo_mechApp", Compute2DTlFiniteStrain);

template<>
InputParameters validParams<Compute2DTlFiniteStrain>()
{
    InputParameters params = validParams<ComputeTlFiniteStrain>();
    params.addClassDescription("Compute strain for finite strain in Total Lagrangian for 2D geometries");
   
    MooseEnum outOfPlaneDirection("x y z","z");
    params.addParam<MooseEnum>(
      "out_of_plane_direction", outOfPlaneDirection, "The direction of the out-of-plane strain.");
    return params;    
}

/**
 * ComputePlaneFiniteStrain defines strain for 2D plane problems
 */

Compute2DTlFiniteStrain::Compute2DTlFiniteStrain(const InputParameters &parameters)
   : ComputeTlFiniteStrain(parameters),
     _out_of_plane_direction(getParam<MooseEnum>("out_of_plane_direction"))
{
}

void
Compute2DTlFiniteStrain::initialSetup()
{
  for (unsigned int i = 0; i < 3; ++i)
  {
    if (_out_of_plane_direction == i)
    {
      _disp[i] = &_zero;
      _grad_disp[i] = &_grad_zero;
    }
    else
    {
      _disp[i] = &coupledValue("displacements", i);
      _grad_disp[i] = &coupledGradient("displacements", i);
    }

  }
}

void
Compute2DTlFiniteStrain::computeProperties()
{
  for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
  {
      RankTwoTensor A((*_grad_disp[0])[_qp],
                      (*_grad_disp[1])[_qp],
                      (*_grad_disp[2])[_qp]);
      _deformation_gradient[_qp] = A;
      _deformation_gradient[_qp].addIa(1.0);
  }
}

void
Compute2DTlFiniteStrain::displacementIntegrityCheck()
{
  if (_out_of_plane_direction != 2 && _ndisp != 3)
    mooseError("For 2D simulations where the out-of-plane direction is x or y the number of "
               "supplied displacements must be three.");
  else if (_out_of_plane_direction == 2 && _ndisp != 2)
    mooseError("For 2D simulations where the out-of-plane direction is z the number of supplied "
                       "displacements must be two.");

}