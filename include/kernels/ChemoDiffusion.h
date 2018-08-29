/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ChemoDiffusion.h
 * Author: srinath
 *
 * Created on August 21, 2018, 8:06 AM
 */

#ifndef CHEMODIFFUSION_H
#define CHEMODIFFUSION_H

#include "Diffusion.h"
#include "Material.h"

//Forward Declaration
class ChemoDiffusion;

template <>
InputParameters validParams<ChemoDiffusion>();

class ChemoDiffusion : public Diffusion
{
public:
    ChemoDiffusion(const InputParameters & parameters);
protected:
    virtual Real computeQpResidual();
    virtual Real computeQpJacobian();

    virtual Real computeQpOffDiagJacobian(unsigned int jvar);
    
    const bool _mu_coupled;
    const unsigned int _mu_var;
    
private:
    
    const MaterialProperty<Real> & _diffusion_coefficient;
    const MaterialProperty<Real> * const _diffusion_coefficient_dC;
    const MaterialProperty<Real> & _activity_coefficient;
    const MaterialProperty<Real> * const _activity_coefficient_dC;
    const MaterialProperty<Real> & _mobility;
    const VariableGradient * _grad_mu;
    
};

#endif /* CHEMODIFFUSION_H */

