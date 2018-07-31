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

template<>
InputParameters validParams<DiffusionMaterial>();


class DiffusionMaterial : public Material 
{
public:
    DiffusionMaterial(const InputParameters & parameters);
protected:
    virtual void computeProerties();
    const Real _M;
    const Real _beta;
    const Real _rhoh;
    MaterialProperty<Real> & _mobility;
    MaterialProperty<Real> & _lattice_misfit;
    MaterialProperty<Real> & _molar_volume;
    
//    MaterialProperty<Real> & _deformation_gradient_inelastic;

};

#endif /* DIFFUSIONMATERIAL_H */

