/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ComputeKirchoffStress.h
 * Author: srinath
 *
 * Created on August 20, 2018, 4:26 PM
 */

#ifndef COMPUTEKIRCHOFFSTRESS_H
#define COMPUTEKIRCHOFFSTRESS_H

#include "ComputeFiniteStrainElasticStress.h"

//Forward Declaration
class ComputeKirchoffStress;

template <>
InputParameters validParams<ComputeKirchoffStress>();

class ComputeKirchoffStress : public ComputeFiniteStrainElasticStress 

{
public:
    ComputeKirchoffStress(const InputParameters & parameters);
protected:
    virtual void computeQpStress();
    virtual void initQpStatefulProperties() override;
    
    const MaterialProperty<RankTwoTensor> & _deformation_gradient;
    MaterialProperty<RankTwoTensor> & _kirchoff_stress;
    
private:

};
#endif /* COMPUTEKIRCHOFFSTRESS_H */

