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
  InputParameters params = validParams<ComputeFiniteStrainStress>();
  params.addClassDescription("Computes stress based on lagrangian strain for Neo-Hookean Material");
  return params;
}



ComputeNeoHookeanStress::ComputeNeoHookeanStress(const InputParameters & parameters)
  : ComputeFiniteStrainStress(parameters)
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
    
      // Calculate the stress in the intermediate configuration
  RankTwoTensor intermediate_stress;

  intermediate_stress =
      _elasticity_tensor[_qp] * (_elastic_strain_old[_qp] + _strain_increment[_qp]);

  // Rotate the stress state to the current configuration
  _stress[_qp] =
      _rotation_increment[_qp] * intermediate_stress * _rotation_increment[_qp].transpose();

  // Assign value for elastic strain, which is equal to the mechanical strain
  _elastic_strain[_qp] = _mechanical_strain[_qp];

  // Compute dstress_dstrain
  _Jacobian_mult[_qp] = _elasticity_tensor[_qp]; // This is NOT the exact jacobian
    
    
    
}
