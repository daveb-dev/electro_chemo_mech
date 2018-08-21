/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ChemoDiffusionTimeDerivative.h
 * Author: srinath
 *
 * Created on August 21, 2018, 9:41 AM
 */

#ifndef CHEMODIFFUSIONTIMEDERIVATIVE_H
#define CHEMODIFFUSIONTIMEDERIVATIVE_H

#include "TimeDerivative.h"
#include "Material.h"

//Forward Declarations
class ChemoDiffusionTimeDerivative;

template <>
InputParameters validParams<ChemoDiffusionTimeDerivative>();

class ChemoDiffusionTimeDerivative : public TimeDerivative
{
public:
    ChemoDiffusionTimeDerivative(const InputParameters & parameters);
    
protected:
//    virtual Real ComputeQpResidual();
//    virtual Real ComputeQpJacobian();
private:
   
};

#endif /* CHEMODIFFUSIONTIMEDERIVATIVE_H */

