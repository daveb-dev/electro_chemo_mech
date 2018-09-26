/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ComputeConcentrationFunctionEigenstrain.h
 * Author: srinath
 *
 * Created on September 20, 2018, 7:53 AM
 */

#ifndef COMPUTECONCENTRATIONFUNCTIONEIGENSTRAIN_H
#define COMPUTECONCENTRATIONFUNCTIONEIGENSTRAIN_H

#include "ComputeConcentrationEigenstrainBase.h"
#include "DerivativeMaterialInterface.h"

class ComputeConcentrationFunctionEigenstrain;

template<>
InputParameters validParams<ComputeConcentrationFunctionEigenstrain>();

class ComputeConcentrationFunctionEigenstrain
    : public ComputeConcentrationEigenstrainBase
{
public:
    ComputeConcentrationFunctionEigenstrain( const InputParameters & parameters);
protected:
    virtual void computeConcentrationStrain(Real & concentration_strain, Real & partial_molar_volume) override;

    Function & _volume_expansion_function;
    Function & _partial_molar_volume_function;
private:

};

#endif /* COMPUTECONCENTRATIONFUNCTIONEIGENSTRAIN_H */

