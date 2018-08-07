/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ComputeConcentrationEigenstrain.h
 * Author: srinath
 *
 * Created on August 7, 2018, 7:51 AM
 */

#ifndef COMPUTECONCENTRATIONEIGENSTRAIN_H
#define COMPUTECONCENTRATIONEIGENSTRAIN_H

#include "ComputeConcentrationEigenstrainBase.h"
#include "DerivativeMaterialInterface.h"

// Forward Declration
class ComputeConcentrationEigenstrain;

template<>
InputParameters validParams<ComputeConcentrationEigenstrain>();

class ComputeConcentrationEigenstrain : public ComputeConcentrationEigenstrainBase

{
public:
    ComputeConcentrationEigenstrain(const InputParameters & parameters);
protected:
    virtual void computeConcentrationStrain(Real & concentration_strain, Real & partial_molar_volume) override;

    const Real & _partial_molar_volume;
private:

};

#endif /* COMPUTECONCENTRATIONEIGENSTRAIN_H */

