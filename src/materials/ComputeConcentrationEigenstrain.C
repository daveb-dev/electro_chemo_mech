/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ComputeConcentrationEigenstrain.C
 * Author: srinath
 * 
 * Created on August 7, 2018, 7:51 AM
 */

#include "ComputeConcentrationEigenstrain.h"

registerMooseObject("TensorMechanicsApp", ComputeConcentrationEigenstrain);

template<>
InputParameters
validParams<ComputeConcentrationEigenstrain> ()
{
    InputParameters params = validParams<ComputeConcentrationEigenstrainBase>();
    
    params.addClassDescription("Computes eigenstrain due to concentration "
            "expansion/shrinkage with a constant partial molar volume");
    params.addRequiredParam<Real>("partial_molar_volume", "Partial molar volume of Li");
    
    return params;
    
}

ComputeConcentrationEigenstrain::ComputeConcentrationEigenstrain(const InputParameters& parameters) 
        : ComputeConcentrationEigenstrainBase(parameters),
        _partial_molar_volume(getParam<Real>("partial_molar_volume"))
{
    
}

void
ComputeConcentrationEigenstrain::computeConcentrationStrain(Real& concentration_strain, Real& partial_molar_volume)
{
    Real vol_strain = 1.0 + _partial_molar_volume*(_concentration[_qp]- _stress_free_concentration[_qp]); // Beta/J_c
    concentration_strain = _partial_molar_volume/(3.0*vol_strain)*(_concentration[_qp]- _stress_free_concentration[_qp]);
    partial_molar_volume = _partial_molar_volume/(3.0*vol_strain);
}        