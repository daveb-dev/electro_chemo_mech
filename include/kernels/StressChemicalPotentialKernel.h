/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   StressChemicalPotentialKernel.h
 * Author: srinath
 *
 * Created on August 21, 2018, 4:13 PM
 */

#ifndef STRESSCHEMICALPOTENTIALKERNEL_H
#define STRESSCHEMICALPOTENTIALKERNEL_H

#include "Kernel.h"
#include "Material.h"
#include "RankTwoTensor.h"
#include "RankFourTensor.h"
#include "DerivativeMaterialInterface.h"

// Forward Declraration

template<>
InputParameters validParams<StressChemicalPotentialKernel>();

class StresschemicalPotentialKernel : public DerivativeMaterialInterface<Kernel>
{
public:
    StresschemicalPotentialKernel(const InputParameters & parameters);
protected:
    virtual Real computeQpResidual();
    virtual Real computeQpJacobian();
    
    const MaterialProperty<RankTwoTensor> * _kirchoff_stress;
       
    
    const MaterialProperty<RankTwoTensor> *_deigenstrain_dc;
    
};



#endif /* STRESSCHEMICALPOTENTIALKERNEL_H */

