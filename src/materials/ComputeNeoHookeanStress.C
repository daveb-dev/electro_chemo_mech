/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ComputeNeoHookeanStress.C
 * Author: srinath
 * 
 * Created on July 9, 2018, 11:45 AM
 */

#include "ComputeNeoHookeanStress.h"
#include "ElasticityTensorTools.h"

registerMooseObject("electro_chemo_mechApp", ComputeNeoHookeanStress);
template <>
InputParameters
validParams<ComputeNeoHookeanStress>()
{
  InputParameters params = validParams<ComputeFiniteStrainElasticStress>();
  params.addClassDescription("Computes stress based on lagrangian strain for Neo-Hookean Material");
  return params;
}



ComputeNeoHookeanStress::ComputeNeoHookeanStress(const InputParameters & parameters)
  : ComputeFiniteStrainElasticStress(parameters),
  _deformation_gradient(getMaterialProperty<RankTwoTensor>(_base_name + "deformation_gradient"))
{
}

void
ComputeNeoHookeanStress::computeQpStress()
{
    const RankTwoTensor II(RankTwoTensor::initIdentity);
    Real J = _deformation_gradient[_qp].det();
    Real Jinv = 1.0/J;
    Real logJ = std::log(J);
    Real mu0 = ElasticityTensorTools::getIsotropicShearModulus(_elasticity_tensor[_qp]);
    Real K = ElasticityTensorTools::getIsotropicBulkModulus(_elasticity_tensor[_qp]);
    Real lambda0 = K - 2.0*mu0/3.0;
    Real mu = mu0 - lambda0*logJ;
    RankTwoTensor B = _deformation_gradient[_qp]*_deformation_gradient[_qp].transpose();
    
    RankFourTensor elast = Jinv*(lambda0*II.outerProduct(II) + mu*(II.mixedProductIkJl(II) + II.mixedProductIlJk(II.transpose())));
    // Belytscho box 5.1 page 245 2003 edition and other paper 2016 Kim (Acta Mech)
       

    //    RankTwoTensor B = _deformation_gradient[_qp]*_deformation_gradient[_qp].transpose();
    _stress[_qp] = Jinv*(lambda0*logJ*II + mu0*(B-II));

  // Assign value for elastic strain, which is equal to the mechanical strain
  _elastic_strain[_qp] = _mechanical_strain[_qp];

  // Compute dstress_dstrain
  _Jacobian_mult[_qp] = elast; // This is NOT the exact jacobian, missing the geometric part perhaps ??
    
    
    
}
