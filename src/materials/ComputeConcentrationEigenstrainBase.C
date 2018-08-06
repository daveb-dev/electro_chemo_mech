/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ComputeDiffusionStrain.C
 * Author: srinath
 * 
 * Created on July 26, 2018, 9:59 AM
 */

#include "ComputeConcentrationEigenstrainBase.h"
#include "RankTwoTensor.h"

// Do not register this class because only the inherited classes can be called
//registerMooseObject("electro_chemo_mechApp", ComputeConcentrationEigenstrainbase);

template<>
InputParameters 
validParams<ComputeConcentrationEigenstrainbase>()
{
    InputParameters params = validParams<ComputeEigenstrainBase>();
    params.addRequiredCoupledVar("concentration","Concentration variable") ;
    return params;
}

ComputeConcentrationEigenstrainbase::ComputeConcentrationEigenstrainbase(const InputParameters& parameters)
        : DerivativeMaterialInterface<ComputeEigenstrainBase>(parameters),
        _conc(coupledValue("concentration")),
        _deigenstrain_dc(declarePropertyDerivative<RankTwoTensor>(_eigenstrain_name,getVar("concentration",0)->name())),
{
    
}        


void
ComputeConcentrationEigenStrainBase::computeQpEigenStrain()
{
    Real concentration_strain = 0.0;
    Real expansion_coeff = 0.0;
    
    computeThermalStrain(concentration_strain, expansion_coeff);
    
    _eigenstrain[_qp].zero();
    _eigenstrain[_qp].zero();
    _eigenstrain[_qp].addIa(thermal_strain);

    _deigenstrain_dc[_qp].zero();
    _deigenstrain_dc[_qp].addIa(expansion_coeff);
    
}