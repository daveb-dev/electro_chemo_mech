/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IsotropicHyperViscoStressUpdate.C
 * Author: srinath
 * 
 * Created on March 18, 2019, 8:27 AM
 */

#include "IsotropicHyperViscoStressUpdate.h"

#include "Function.h"
#include "ElasticityTensorTools.h"

#include "libmesh/quadrature.h"
#include "libmesh/utility.h"

registerMooseObject("electro_chemo_mechApp", IsotropicHyperViscoStressUpdate);

template <>
InputParameters
validParams<IsotropicHyperViscoStressUpdate>()
{
  InputParameters params = validParams<RadialReturnStressUpdate>();
  params.addClassDescription("This class uses the discrete material in a radial return isotropic "
                             "plasticity model.  This class is one of the basic radial return "
                             "constitutive models, yet it can be used in conjunction with other "
                             "creep and plasticity materials for more complex simulations.");
  
  params.addRequiredParam<Real>("rate_exponent", "Rate Sensitivity parameter");
  params.addRequiredParam<Real>("reference_strain_rate", "Reference strain rate");
  params.addRequiredParam<Real>("initial_resistance", "Initial resistance in tension");
  params.addRequiredParam<Real>("hardening_modulus", "Hardening modulus in tension");
  params.addRequiredParam<Real>("saturation_resistance", "Saturation value of hardening in tension");
  params.addRequiredParam<Real>("hardening_exponent", "Hardening exponent");
  params.addDeprecatedParam<std::string>(
      "plastic_prepend",
      "",
      "String that is prepended to the plastic_strain Material Property",
      "This has been replaced by the 'base_name' parameter");
  params.set<std::string>("effective_inelastic_strain_name") = "effective_plastic_strain";

  return params;
}

IsotropicHyperViscoStressUpdate::IsotropicHyperViscoStressUpdate(const InputParameters & parameters)
  : RadialReturnStressUpdate(parameters),
    _plastic_prepend(getParam<std::string>("plastic_prepend")),
    _mrate(getParam<Real>("rate_exponent")),
    _epsilon_rate(getParam<Real>("reference_strain_rate")),
    _Y0(getParam<Real>("initial_resistance")),
    _H0(getParam<Real>("hardening_modulus")),
    _Ysat(getParam<Real>("saturation_resistance")),
    _ahard(getParam<Real>("hardening_exponent")),
    _plastic_strain(
        declareProperty<RankTwoTensor>(_base_name + _plastic_prepend + "plastic_strain")),
    _plastic_strain_old(
        getMaterialPropertyOld<RankTwoTensor>(_base_name + _plastic_prepend + "plastic_strain")),
    _yield_strength(declareProperty<Real>(_base_name + "yield_strength")),
    _hardening_variable(declareProperty<Real>(_base_name + "hardening_variable")),
    _hardening_variable_old(getMaterialPropertyOld<Real>(_base_name + "hardening_variable")), 
    _strength_variable(declareProperty<Real>(_base_name + "strength_variable")),
    _strength_variable_old(getMaterialPropertyOld<Real>(_base_name + "strength_variable")), 
    _deformation_gradient(getMaterialProperty<RankTwoTensor>(_base_name + "deformation_gradient")),
    _deformation_gradient_old(getMaterialPropertyOld<RankTwoTensor>(_base_name + "deformation_gradient")),
    _Fp(declareProperty<RankTwoTensor>(_base_name + _plastic_prepend + "plastic_distortion")),        
    _Fp_old(getMaterialPropertyOld<RankTwoTensor>(_base_name + _plastic_prepend + "plastic_distortion"))

{
    _shear_rate = _epsilon_rate * std::sqrt(3.0);
    _shear_initial_resistance = _Y0 / std::sqrt(3.0);
    _shear_saturation = _Ysat / std::sqrt(3.0);
    _shear_initial_hardness = _H0 / 3.0;
}

void
IsotropicHyperViscoStressUpdate::initQpStatefulProperties()
{
  _hardening_variable[_qp] = _shear_initial_hardness;
  _strength_variable[_qp] = _shear_initial_resistance;
  _yield_strength[_qp] = _Y0;
  _plastic_strain[_qp].zero();
  _Fp[_qp].zero();
  _Fp[_qp].addIa(1.0);
  RadialReturnStressUpdate::initQpStatefulProperties();
}

void
IsotropicHyperViscoStressUpdate::propagateQpStatefulProperties()
{
  _hardening_variable[_qp] = _hardening_variable_old[_qp];
  _strength_variable[_qp] = _strength_variable_old[_qp];
  _plastic_strain[_qp] = _plastic_strain_old[_qp];
  _Fp[_qp] = _Fp_old[_qp];
  propagateQpStatefulPropertiesRadialReturn();
}


Real
IsotropicHyperViscoStressUpdate::minimumPermissibleValue(const Real effective_trial_stress) const
{
    return 0.0;
}

void
IsotropicHyperViscoStressUpdate::computeStressInitialize(const Real effective_trial_stress,
                                                         const RankFourTensor & elasticity_tensor)
{
  computeYieldStress(elasticity_tensor);
  _hardening_variable[_qp] = _shear_initial_hardness 
                            * std::pow((1.0 - (_strength_variable_old[_qp] / _shear_saturation)), _ahard);

}

Real
IsotropicHyperViscoStressUpdate::computeResidual(const Real effective_trial_stress,
                                                 const Real scalar)
{
  Real residual = 0.0;

  _strength_variable[_qp] = computeHardeningValue(scalar);
   residual = effective_trial_stress - _dt * scalar *_three_shear_modulus / 3.0 
           - _strength_variable[_qp] * (std::pow((scalar/_shear_rate), _mrate));
  
//  if (scalar > 0.0)
//  {
//    residual = effective_trial_stress - _dt * scalar *_three_shear_modulus / 3.0 
//           - _strength_variable[_qp] * (std::pow((scalar/_shear_rate), _mrate));
//  } else
//  {
//      residual =  effective_trial_stress - _dt * scalar *_three_shear_modulus / 3.0; 
//  }
  return residual;
}

Real
IsotropicHyperViscoStressUpdate::computeDerivative(const Real effective_trial_stress,
                                                   const Real scalar)
{
  Real derivative;
  Real fac1 = 0.0;
  Real fac2 = 0.0;
         
  
  if (scalar <= 0.0)
  {
        fac1 = 0.0;
        fac2 = 0.0;
        
  } else
  {
        Real hslope = computeHardeningDerivative(scalar);
        fac1 = hslope * std::pow((scalar/_shear_rate), _mrate);
        fac2 = _strength_variable[_qp] * (_mrate/_shear_rate) * 
                   std::pow((scalar / _shear_rate), _mrate-1.0);
  }
  derivative = -_three_shear_modulus / 3.0 * _dt - fac1 - fac2;
  return derivative;
}

void
IsotropicHyperViscoStressUpdate::iterationFinalize(Real scalar)
{
    _strength_variable[_qp] = _strength_variable_old[_qp] + _dt * _hardening_variable[_qp] * scalar;
}

void
IsotropicHyperViscoStressUpdate::computeStressFinalize(const RankTwoTensor & plasticStrainIncrement)
{
  _plastic_strain[_qp] += plasticStrainIncrement;
  _yield_strength[_qp] = _strength_variable[_qp]*std::sqrt(3.0);
  
}

Real
IsotropicHyperViscoStressUpdate::computeHardeningValue(Real scalar)
{
  Real value = 0.0;
  
  value = _strength_variable_old[_qp] + _dt * _hardening_variable[_qp] * scalar;
//  _strength_variable[_qp] = value;
  return value;
}

Real IsotropicHyperViscoStressUpdate::computeHardeningDerivative(Real /*scalar*/)
{
  Real slope = 0.0;
  slope = _hardening_variable[_qp] * _dt;
  return slope;
}

void
IsotropicHyperViscoStressUpdate::computeYieldStress(const RankFourTensor & /*elasticity_tensor*/)
{ 
}

void
IsotropicHyperViscoStressUpdate::updateState(RankTwoTensor & strain_increment,
                                      RankTwoTensor & inelastic_strain_increment,
                                      const RankTwoTensor & /*rotation_increment*/,
                                      RankTwoTensor & stress_new,
                                      const RankTwoTensor & /*stress_old*/,
                                      const RankFourTensor & elasticity_tensor,
                                      const RankTwoTensor & elastic_strain_old,
                                      bool compute_full_tangent_operator,
                                      RankFourTensor & tangent_operator)
{
    RankTwoTensor Fp_inv (_Fp_old[_qp].inverse());
    RankTwoTensor Fe_tr = _deformation_gradient[_qp]*Fp_inv;
    
    RankTwoTensor Ce_tr = Fe_tr.transpose() * Fe_tr; 
    std::vector<Real> e_value(3);
    RankTwoTensor e_vector, N1, N2, N3;
    Ce_tr.symmetricEigenvaluesEigenvectors(e_value, e_vector);
    const Real lambda1 = std::sqrt(e_value[0]);
    const Real lambda2 = std::sqrt(e_value[1]);
    const Real lambda3 = std::sqrt(e_value[2]);

    N1.vectorOuterProduct(e_vector.column(0), e_vector.column(0));
    N2.vectorOuterProduct(e_vector.column(1), e_vector.column(1));
    N3.vectorOuterProduct(e_vector.column(2), e_vector.column(2));
    
    RankTwoTensor Ue_tr = N1 * lambda1 + N2 * lambda2 + N3 * lambda3;
    RankTwoTensor invUe(Ue_tr.inverse());
    RankTwoTensor Ee_tr = N1 * std::log(lambda1) + N2 * std::log(lambda2) + N3 * std::log(lambda3);
    RankTwoTensor R_tr = Fe_tr * invUe;
    
    // Mandel Stress trial
    RankTwoTensor Me_tr = elasticity_tensor * Ee_tr;
    RankTwoTensor deviatoric_trial_stress = Me_tr.deviatoric();
 
    // compute the effective trial stress
  Real dev_trial_stress_squared =
      deviatoric_trial_stress.doubleContraction(deviatoric_trial_stress);
  Real effective_trial_stress = std::sqrt(1.0 / 2.0 * dev_trial_stress_squared);

  // Set the value of 3 * shear modulus for use as a reference residual value
  _three_shear_modulus = 3.0 * ElasticityTensorTools::getIsotropicShearModulus(elasticity_tensor);

  computeStressInitialize(effective_trial_stress, elasticity_tensor);

  // Use Newton iteration to determine the scalar effective inelastic strain increment
  Real scalar_effective_inelastic_strain = 0.0;
  if (!MooseUtils::absoluteFuzzyEqual(effective_trial_stress, 0.0))
  {
    returnMappingSolve(effective_trial_stress, scalar_effective_inelastic_strain, _console);
    if (scalar_effective_inelastic_strain != 0.0)
      inelastic_strain_increment =
          (0.5) * deviatoric_trial_stress *
          (scalar_effective_inelastic_strain / effective_trial_stress);
    else
      inelastic_strain_increment.zero();
  }
  else
    inelastic_strain_increment.zero();

  strain_increment -= inelastic_strain_increment;
  _effective_inelastic_strain[_qp] = 
      _effective_inelastic_strain_old[_qp] + _dt * scalar_effective_inelastic_strain;

//  _effective_inelastic_strain[_qp] = scalar_effective_inelastic_strain;
  
//  // Compute new Cauchy stress
  
  RankTwoTensor dpt = inelastic_strain_increment * _dt;
  dpt.symmetricEigenvaluesEigenvectors(e_value, e_vector);
  
  N1.vectorOuterProduct(e_vector.column(0), e_vector.column(0));
  N2.vectorOuterProduct(e_vector.column(1), e_vector.column(1));
  N3.vectorOuterProduct(e_vector.column(2), e_vector.column(2));
  RankTwoTensor expDp = N1 * std::exp(e_value[0]) + 
                        N2 * std::exp(e_value[1]) + 
                        N3 * std::exp(e_value[2]);  
  _Fp[_qp] = expDp * _Fp_old[_qp];
  Fp_inv = _Fp[_qp].inverse();
  Fe_tr = _deformation_gradient[_qp] * Fp_inv;
  
  Ce_tr = Fe_tr.transpose() * Fe_tr; 
  Ce_tr.symmetricEigenvaluesEigenvectors(e_value, e_vector);
  const Real l1 = std::sqrt(e_value[0]);
  const Real l2 = std::sqrt(e_value[1]);
  const Real l3 = std::sqrt(e_value[2]);

   N1.vectorOuterProduct(e_vector.column(0), e_vector.column(0));
   N2.vectorOuterProduct(e_vector.column(1), e_vector.column(1));
   N3.vectorOuterProduct(e_vector.column(2), e_vector.column(2));
    
  Ue_tr = N1 * l1 + N2 * l2 + N3 * l3;
  invUe= Ue_tr.inverse();
  Ee_tr = N1 * std::log(l1) + N2 * std::log(l2) + N3 * std::log(l3);
  R_tr = Fe_tr * invUe;
  
  RankTwoTensor Me_tau = Me_tr - (2.0/3.0)*_three_shear_modulus 
                         * _dt * inelastic_strain_increment;
  
  stress_new = R_tr * (Me_tau * R_tr.transpose()) / _deformation_gradient[_qp].det();
  
  computeStressFinalize(inelastic_strain_increment);


  if (compute_full_tangent_operator &&
      getTangentCalculationMethod() == TangentCalculationMethod::PARTIAL)
  {
    if (MooseUtils::absoluteFuzzyEqual(scalar_effective_inelastic_strain, 0.0))
      tangent_operator.zero();
    else
    {
      // mu = _three_shear_modulus / 3.0;
      // norm_dev_stress = ||s_n+1||
      // effective_trial_stress = von mises trial stress = std::sqrt(3.0 / 2.0) * ||s_n+1^trial||
      // scalar_effective_inelastic_strain = Delta epsilon^cr_n+1
      // deriv = derivative of scalar_effective_inelastic_strain w.r.t. von mises stress
      // deriv = std::sqrt(3.0 / 2.0) partial Delta epsilon^cr_n+1n over partial ||s_n+1^trial||

      mooseAssert(_three_shear_modulus != 0.0, "Shear modulus is zero");

      const RankTwoTensor deviatoric_stress = stress_new.deviatoric();
      const Real norm_dev_stress =
          std::sqrt(deviatoric_stress.doubleContraction(deviatoric_stress));
      mooseAssert(norm_dev_stress != 0.0, "Norm of the deviatoric is zero");

      const RankTwoTensor flow_direction = deviatoric_stress / norm_dev_stress;
      const RankFourTensor flow_direction_dyad = flow_direction.outerProduct(flow_direction);
      const Real deriv =
          computeStressDerivative(effective_trial_stress, scalar_effective_inelastic_strain);
      const Real scalar_one = _three_shear_modulus * scalar_effective_inelastic_strain /
                              std::sqrt(1.5) / norm_dev_stress;

      tangent_operator = scalar_one * _deviatoric_projection_four +
                         (_three_shear_modulus / 3.0 * deriv - scalar_one) * flow_direction_dyad;
    }
  } 
  
}

Real
IsotropicHyperViscoStressUpdate::computeReferenceResidual(const Real effective_trial_stress,
                                                   const Real scalar_effective_inelastic_strain)
{
  return effective_trial_stress / _three_shear_modulus /3.0 - scalar_effective_inelastic_strain;
}


Real
IsotropicHyperViscoStressUpdate::computeStressDerivative(const Real effective_trial_stress, 
                                                    const Real scalar)
{
    Real fac2; 
    Real ret = 0.0;
    if (effective_trial_stress > 0.0 && scalar > 0)
    {
        fac2 = 2.0/3.0*_three_shear_modulus/3.0*_dt;
        
        
        Real hslope = computeHardeningDerivative(scalar);
        Real qq = std::pow((scalar/_shear_rate), _mrate);
        
        Real den1 = hslope * qq;
        Real den2 = _strength_variable[_qp] * _mrate / scalar * qq;
        ret = fac2/(den1 + den2);

    }
    return ret;
}