/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ComputeDiffusionStrain.h
 * Author: srinath
 *
 * Created on July 26, 2018, 9:59 AM
 */

#ifndef COMPUTECONCENTRATIONEIGENSTRAINBASE_H
#define COMPUTECONCENTRATIONEIGENSTRAINBASE_H

#include "Material.h"
#include "RankTwoTensor.h"
#include "RankFourTensor.h"
#include "RotationTensor.h"
#include "DerivativeMaterialInterface.h"
#include "ComputeEigenstrainBase.h"

//Forward Declaration
class ComputeConcentrationEigenstrainBase;

template <>
InputParameters validParams<ComputeConcentrationEigenstrainBase>();

class ComputeConcentrationEigenstrainBase 
    : public DerivativeMaterialInterface<ComputeEigenstrainBase>
{
public:
    ComputeConcentrationEigenstrainBase(const InputParameters & parameters);
protected:
    virtual void computeQpEigenstrain() override;

    virtual void computeConcentrationStrain(Real & conc_strain, Real & expansion_coeff) = 0;
    
    const VariableValue & _concentration;   
    MaterialProperty<RankTwoTensor> & _deigenstrain_dc;
    
    
private:
    

};

#endif /* COMPUTECONCENTRATIONEIGENSTRAINBASE_H */

