/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ComputeConcentrationFunctionEigenstrain.C
 * Author: srinath
 * 
 * Created on September 20, 2018, 7:53 AM
 */

#include "ComputeConcentrationFunctionEigenstrain.h"
#include "Function.h"

registerMooseObject("electro_chemo_mechApp", ComputeConcentrationFunctionEigenstrain);

template<>
InputParameters
validParams<ComputeConcentrationFunctionEigenstrain>()
{
    InputParameters params = validParams<ComputeConcentrationEigenstrainBase>();
    params.addClassDescription("Computes eigenstrain due to a function");
    params.addRequiredParam<FunctionName>("volume_expansion_function","Function");
    params.addRequiredParam<FunctionName>("partial_molar_volume_function","Cunction");
    return params;
}

ComputeConcentrationFunctionEigenstrain::
    ComputeConcentrationFunctionEigenstrain(const InputParameters& parameters)
    :ComputeConcentrationEigenstrainBase(parameters),
    _volume_expansion_function(getFunction("volume_expansion_function")),
    _partial_molar_volume_function(getFunction("partial_molar_volume_function"))
{
}

void
ComputeConcentrationFunctionEigenstrain::
        computeConcentrationStrain( Real & concentration_strain, Real & partial_molar_volume)
{
    const Real & conc = _concentration[_qp];
    const Point p;
    const Real cstrain = _volume_expansion_function.value(conc, p);
    const Real pvol = _partial_molar_volume_function.value(conc, p);
    concentration_strain = (1.0/3.0)*std::log(cstrain);
    partial_molar_volume = pvol/(3.0*cstrain);
}        