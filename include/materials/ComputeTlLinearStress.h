/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ComputeTLNeoHookeanPkStress.h
 * Author: srinath
 *
 * Created on July 24, 2018, 10:07 AM
 */

#ifndef COMPUTETLLINEARSTRESS_H
#define COMPUTETLLINEARPKSTRESS_H

#include "ComputeTlFiniteStrainStress.h"

//Forward Declaration
class ComputeTLNeoHookeanPkStress;

template <>
InputParameters validParams<ComputeTLNeoHookeanPkStress>();

class ComputeTLNeoHookeanPkStress : public ComputeTlFiniteStrainStress 

{
public:
    ComputeTLNeoHookeanPkStress(const InputParameters & parameters);
protected:
    virtual void computeQpStress();
private:

};

#endif /* COMPUTETLNEOHOOKEANPKSTRESS_H */

