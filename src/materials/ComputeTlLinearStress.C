/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ComputeTLNeoHookeanPkStress.cpp
 * Author: srinath
 * 
 * Created on July 24, 2018, 10:07 AM
 */

#include "ComputeTLNeoHookeanPkStress.h"
#include "ElasticityTensorTools.h"

registerMooseObject("electro_chemo_mechApp", ComputeTLNeoHookeanPkStress);
template <>
InputParameters
validParams<ComputeTLNeoHookeanPkStress>()
{
  InputParameters params = validParams<ComputeTlFiniteStrainStress>();
  params.addClassDescription("Computes stress based on lagrangian strain for Neo-Hookean Material");
  return params;
}



ComputeTLNeoHookeanPkStress::ComputeTLNeoHookeanPkStress(const InputParameters & parameters)
  : ComputeTlFiniteStrainStress(parameters)
{
}

void
ComputeTLNeoHookeanPkStress::computeQpStress()
{
    const RankTwoTensor II(RankTwoTensor::initIdentity);
    const RankFourTensor I(RankFourTensor::initIdentity);
    Real Je = _deformation_gradient[_qp].det();
    Real logJ = std::log(Je);
    Real mu0 = ElasticityTensorTools::getIsotropicShearModulus(_elasticity_tensor[_qp]);
    Real K = ElasticityTensorTools::getIsotropicBulkModulus(_elasticity_tensor[_qp]);
    Real lambda0 = K - 2.0*mu0/3.0;
    RankTwoTensor Finv_e = _deformation_gradient[_qp].inverse();
    RankTwoTensor Finv_eT = Finv_e.transpose();    
    RankTwoTensor Pk1 = (lambda0*logJ - mu0)*Finv_eT + mu0*_deformation_gradient[_qp];
    
    _stress[_qp] = (1.0/Je)*Pk1*_deformation_gradient[_qp].transpose();    
    _Jacobian_mult[_qp] = lambda0*(Finv_eT.outerProduct(Finv_eT)) - (lambda0*logJ - mu0)*(0.5*(Finv_e.mixedProductIkJl(Finv_e) + Finv_e.mixedProductIlJk(Finv_e))) + mu0*I;
}

