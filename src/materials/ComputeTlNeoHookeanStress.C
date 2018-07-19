/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ComputeTlNeoHookeanStress.C
 * Author: srinath
 * 
 * Created on July 9, 2018, 11:45 AM
 */

#include "ComputeTlNeoHookeanStress.h"
#include "ElasticityTensorTools.h"

registerMooseObject("electro_chemo_mechApp", ComputeTlNeoHookeanStress);
template <>
InputParameters
validParams<ComputeTlNeoHookeanStress>()
{
  InputParameters params = validParams<ComputeTlFiniteStrainStress>();
  params.addClassDescription("Computes stress based on lagrangian strain for Neo-Hookean Material");
  return params;
}



ComputeTlNeoHookeanStress::ComputeTlNeoHookeanStress(const InputParameters & parameters)
  : ComputeTlFiniteStrainStress(parameters)
{
}

void
ComputeTlNeoHookeanStress::computeQpStress()
{
    const RankTwoTensor II(RankTwoTensor::initIdentity);
    Real J = _deformation_gradient[_qp].det();
    Real logJ = std::log(J);
    Real mu0 = ElasticityTensorTools::getIsotropicShearModulus(_elasticity_tensor[_qp]);
    Real K = ElasticityTensorTools::getIsotropicBulkModulus(_elasticity_tensor[_qp]);
    Real lambda0 = K - 2.0*mu0/3.0;
    //Real mu = mu0 - lambda0*logJ;
    RankTwoTensor B = _deformation_gradient[_qp]*_deformation_gradient[_qp].transpose();
    //RankTwoTensor C = _deformation_gradient[_qp].transpose()*_deformation_gradient[_qp];
    //RankTwoTensor Cinv = C.inverse();
    // Material Portion of the Jacobian matrix 
    _stress[_qp] = (lambda0*logJ*II + mu0*(B-II))/J;
    //_Jacobian_mult[_qp] = lambda0*Cinv.outerProduct(Cinv) + mu*(Cinv.mixedProductIkJl(Cinv) + Cinv.transpose().mixedProductIlJk(Cinv));
    _Jacobian_mult[_qp] = lambda0*(II.outerProduct(II)) + mu0*(II.mixedProductIkJl(B) + B.mixedProductIlJk(II));
}
