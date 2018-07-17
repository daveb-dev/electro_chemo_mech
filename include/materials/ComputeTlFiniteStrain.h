//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef COMPUTETLFINITESTRAINBASE_H
#define COMPUTETLFINITESTRAINBASE_H

#include "ComputeStrainBase.h"

class ComputeTlFiniteStrain;

template <>
InputParameters validParams<ComputeTlFiniteStrain>();

/**
 * ComputeTlFiniteStrainBase defines a Finite strain strain tensor based on Total Lagrangian formulation 
 * 
 */
class ComputeTlFiniteStrain : public ComputeStrainBase
{
public:
  ComputeTlFiniteStrain(const InputParameters & parameters);
  
protected:
    void initialSetup() override;
    virtual void computeProperties() override;
    virtual void initQpStatefulProperties() override;
    MaterialProperty<RankTwoTensor> & _deformation_gradient; // Deformation Gradient Tensor at each quadrature point
  
};

#endif // COMPUTESMALLSTRAIN_H
