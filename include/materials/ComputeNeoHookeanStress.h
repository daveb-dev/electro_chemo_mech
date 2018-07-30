/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ComputeTlNeoHookeanStress.h
 * Author: srinath
 *
 * Created on July 9, 2018, 11:45 AM
 */

#ifndef COMPUTETLNEOHOOKEANSTRESS_H
#define COMPUTETLNEOHOOKEANSTRESS_H

#include "ComputeFiniteStrainStress.h"

//Forward Declaration
class ComputeTlNeoHookeanStress;

template <>
InputParameters validParams<ComputeTlNeoHookeanStress>();

class ComputeTlNeoHookeanStress : public ComputeFiniteStrainStress 

{
public:
    ComputeTlNeoHookeanStress(const InputParameters & parameters);
protected:
    virtual void computeQpStress();
private:

};

#endif /* COMPUTETLNEOHOOKEANSTRESS_H */

