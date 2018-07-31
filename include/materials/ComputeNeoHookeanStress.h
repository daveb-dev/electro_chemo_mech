/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ComputeNeoHookeanStress.h
 * Author: srinath
 *
 * Created on July 9, 2018, 11:45 AM
 */

#ifndef COMPUTENEOHOOKEANSTRESS_H
#define COMPUTENEOHOOKEANSTRESS_H

#include "ComputeFiniteStrainElasticStress.h"

//Forward Declaration
class ComputeNeoHookeanStress;

template <>
InputParameters validParams<ComputeNeoHookeanStress>();

class ComputeNeoHookeanStress : public ComputeFiniteStrainElasticStress 

{
public:
    ComputeNeoHookeanStress(const InputParameters & parameters);
protected:
    virtual void computeQpStress();
    
    const MaterialProperty<RankTwoTensor> & _deformation_gradient;
private:

};

#endif /* COMPUTENEOHOOKEANSTRESS_H */

