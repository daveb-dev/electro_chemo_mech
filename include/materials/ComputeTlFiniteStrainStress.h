//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef COMPUTETLFINITESTRAINSTRESS_H
#define COMPUTETLFINITESTRAINSTRESS_H

#include "ComputeStressBase.h"
#include "GuaranteeConsumer.h"


//Forward Declaration
class ComputeTlFiniteStrainStress;

template <>
InputParameters validParams<ComputeTlFiniteStrainStress>();

/**
 * ComputeDefGradBasedStress computes stress based on lagrangian strain definition
 **/
class ComputeTlFiniteStrainStress :  public ComputeStressBase, public GuaranteeConsumer
{
public:
  ComputeTlFiniteStrainStress(const InputParameters & parameters);
   void initialSetup() override;

protected:
  virtual void initQpStatefulProperties();
  virtual void computeQpStress();

  const MaterialProperty<RankTwoTensor> & _deformation_gradient;

};

#endif
