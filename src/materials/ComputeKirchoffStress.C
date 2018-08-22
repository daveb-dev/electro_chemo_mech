/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ComputeKirchoffStress.C
 * Author: srinath
 * 
 * Created on August 20, 2018, 4:26 PM
 */

#include "ComputeKirchoffStress.h"
#include "ElasticityTensorTools.h"

//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html


registerMooseObject("electro_chemo_mechApp", ComputeKirchoffStress);
template <>
InputParameters
validParams<ComputeKirchoffStress>()
{
  InputParameters params = validParams<ComputeFiniteStrainElasticStress>();
  params.addClassDescription("Computes stress based on lagrangian strain for Kirchoff material");
  return params;
}



ComputeKirchoffStress::ComputeKirchoffStress(const InputParameters & parameters)
  : ComputeFiniteStrainElasticStress(parameters),
  _deformation_gradient(getMaterialProperty<RankTwoTensor>(_base_name + "deformation_gradient")),
  _kirchoff_stress(declareProperty<RankTwoTensor>(_base_name + "kirchoff_stress"))
{
}

void
ComputeKirchoffStress::initQpStatefulProperties()
{
    ComputeStressBase::initQpStatefulProperties();
    _kirchoff_stress[_qp].zero();
}

void
ComputeKirchoffStress::computeQpStress()
{
    Real J = _deformation_gradient[_qp].det();
    Real Jinv = 1.0/J;
    ComputeFiniteStrainElasticStress::computeQpStress();
    // The Constitutive model provides the kirchoff stress and we need to compute the others
    _kirchoff_stress[_qp] = _stress[_qp];
    
    _stress[_qp] *= Jinv;
    _Jacobian_mult[_qp] *= Jinv;
}

