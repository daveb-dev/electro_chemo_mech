/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DiffusionMaterial.h
 * Author: srinath
 *
 * Created on July 25, 2018, 10:06 AM
 */

#ifndef DIFFUSIONMATERIAL_H
#define DIFFUSIONMATERIAL_H

#include "Material.h"


//Forward Declrartion
class DiffusionMaterial;
class Function;

template<>
InputParameters validParams<DiffusionMaterial>();


class DiffusionMaterial : public Material 
{
public:
    DiffusionMaterial(const InputParameters & parameters);
protected:
    virtual void computeProperties();

    const bool _has_conc;
    const VariableValue & _concentration;
    
    // Local variables
    const Real _M;
    const Real _gamma;
    const Real _R; 
    const Real _temp;
    
    // Material Property variables
    MaterialProperty<Real> & _diffusion_coefficient;
    MaterialProperty<Real> & _mobility;
    MaterialProperty<Real> & _diffusion_coefficient_dC;
    Function * _diffusion_coefficient_concentration_function;
    
    MaterialProperty<Real> & _activity_coefficient;
    MaterialProperty<Real> & _activity_coefficient_dC;
    Function * _actitivity_coefficient_concentration_function;

    
};

#endif /* DIFFUSIONMATERIAL_H */

