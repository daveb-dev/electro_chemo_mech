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
validParams<ComputeConcentrationEigenstrainBase>()
{
    InputParameters params = validParams<ComputeEigenstrainBase>();
    params.addRequiredCoupledVar("concentration","Concentration variable") ;
    return params;
}

ComputeConcentrationEigenstrainBase::ComputeConcentrationEigenstrainBase(const InputParameters& parameters)
        : DerivativeMaterialInterface<ComputeEigenstrainBase>(parameters),
        _concentration(coupledValue("concentration")),
        _deigenstrain_dc(declarePropertyDerivative<RankTwoTensor>(_eigenstrain_name,getVar("concentration",0)->name()))
{
    
}        


void
ComputeConcentrationEigenstrainBase::computeQpEigenstrain()
{
    Real concentration_strain = 0.0;
    Real partial_molar_volume = 0.0;
    
    computeConcentrationStrain(concentration_strain, partial_molar_volume);
    
    _eigenstrain[_qp].zero();
    _eigenstrain[_qp].zero();
    _eigenstrain[_qp].addIa(concentration_strain);

    // This is not correct 
    _deigenstrain_dc[_qp].zero();
    _deigenstrain_dc[_qp].addIa(partial_molar_volume);
    
}