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

#ifndef COMPUTEDIFFUSIONSTRAIN_H
#define COMPUTEDIFFUSIONSTRAIN_H

#include "Material.h"
#include "RankTwoTensor.h"
#include "RankFourTensor.h"
#include "RotationTensor.h"
#include "DerivativeMaterialInterface.h"
#include "ComputeEigenstrainBase.h"

//Forward Declaration
class ComputeDiffusionStrain;

template <>
InputParameters validParams<ComputeDiffusionStrain>();

class ComputeDiffusionStrain : public ComputeEigenstrainBase
{
public:
    ComputeDiffusionStrain(const InputParameters & parameters);
protected:
    void initialSetup() override;
    void computeProperties() override;

    
    const VariableValue & _conc;   
    
    MaterialProperty<RankTwoTensor> & _deformation_gradient_diffusion;
    MaterialProperty<RankTwoTensor> & _dFdc;

    const MaterialProperty<Real> & _mobility;
    const MaterialProperty<Real> & _lattice_misfit;
    const MaterialProperty<Real> & _molar_volume;

private:
    

};

#endif /* COMPUTEDIFFUSIONSTRAIN_H */

