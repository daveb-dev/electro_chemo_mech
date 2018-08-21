/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ChemoDiffusionTimeDerivative.C
 * Author: srinath
 * 
 * Created on August 21, 2018, 9:41 AM
 */

#include "ChemoDiffusionTimeDerivative.h"

registerMooseObject("electro_chemo_mechApp",ChemoDiffusionTimeDerivative);

template<>
InputParameters
validParams<ChemoDiffusionTimeDerivative>()
{
    InputParameters params = validParams<TimeDerivative>();
    params.addClassDescription("Time Derivative for Chemo-diffusion problem... TBD");
    params.set<bool>("use_displaced_mesh") = false;
    return params;
}

ChemoDiffusionTimeDerivative::ChemoDiffusionTimeDerivative(const InputParameters& parameters)
                            : TimeDerivative(parameters)
{
}

//Real
//ChemoDiffusionTimeDerivative::computeQpResidual()
//{
//    TimeDerivative::computeQpResidual();
//}
//
//Real
//ChemoDiffusionTimeDerivative::computeQpJacobian()
//{
//    TimeDerivative::computeQpJacobian();
//}