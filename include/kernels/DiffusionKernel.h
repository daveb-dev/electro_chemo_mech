/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DiffusionKernel.h
 * Author: srinath
 *
 * Created on July 25, 2018, 10:48 AM
 */

#ifndef DIFFUSIONKERNEL_H
#define DIFFUSIONKERNEL_H

#include "Diffusion.h"

//Forward Declaration
class DiffusionKernel;

template<>
InputParameters validParams<DiffusionKernel>();

class DiffusionKernel : public Diffusion
{
public:
    DiffusionKernel(const InputParameters & parameters);
    
protected:
    virtual Real computeQpResidual() override;
//    virtual Real computeQpJacobian() override;
    
    const MaterialProperty<Real> & _mobility;
    const MaterialProperty<Real> & _lattice_misfit;
    const MaterialProperty<Real> & _molar_volume;
    const MaterialProperty<Real> & _max_concentration;
    
    const MaterialProperty<RankTwoTensor> & _deformation_gradient;
    const MaterialProperty<RankTwoTensor> & _deformation_gradient_diffusion;
    
    const MaterialProperty<RankTwoTensor> & _stress;
    
};

#endif /* DIFFUSIONKERNEL_H */

