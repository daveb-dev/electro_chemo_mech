/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IsotropicHyperViscoStressUpdate.h
 * Author: srinath
 *
 * Created on March 18, 2019, 8:27 AM
 */

#ifndef ISOTROPICHYPERVISCOSTRESSUPDATE_H
#define ISOTROPICHYPERVISCOSTRESSUPDATE_H

#include "RadialReturnStressUpdate.h"

class IsotropicHyperViscoStressUpdate;

template <>
InputParameters validParams<IsotropicHyperViscoStressUpdate>();

/**
 * This class uses the Discrete material in a radial return isotropic plasticity
 * model.  
 * 
 * This class inherits from RadialReturnStressUpdate and must be used
 * in conjunction with ComputeReturnMappingStress.  This class calculates
 * an effective trial stress, an effective scalar plastic strain
 * increment, and the derivative of the scalar effective plastic strain increment;
 * these values are passed to the RadialReturnStressUpdate to compute
 * the radial return stress increment.  This isotropic plasticity class also
 * computes the plastic strain as a stateful material property.
 *
 * This class uses the implicit integration algorithm in F. Dunne and N.
 * Petrinic's Introduction to Computational Plasticity (2004) Oxford University
 * Press, pg. 146 - 149.
 * This class is based on a total formulation, so the stress increment and 
 * plastic strain increment are bases on 
 * Weber and Anand (1990) Computer Methods in App mech and engr, pg. 173 - 183
 */

class IsotropicHyperViscoStressUpdate : public RadialReturnStressUpdate
{
public:
  IsotropicHyperViscoStressUpdate(const InputParameters & parameters);
  
  /**
   * Here is a specialization of regular stressupdate so that correct stress terms
   * are sent to the radial algorithm
   * A radial return (J2) mapping method is performed with return mapping
   * iterations.
   * @param strain_increment              Sum of elastic and inelastic strain increments
   * @param inelastic_strain_increment    Inelastic strain increment calculated by this class
   * @param rotation increment            Not used by this class
   * @param stress_new                    New trial stress from pure elastic calculation
   * @param stress_old                    Old state of stress
   * @param elasticity_tensor             Rank 4 C_{ijkl}, must be isotropic
   * @param elastic_strain_old            Old state of total elastic strain
   * @param compute_full_tangent_operator Flag currently unused by this class
   * @param tangent_operator              Currently a copy of the elasticity tensor in this class
   */
  virtual void updateState(RankTwoTensor & strain_increment,
                           RankTwoTensor & inelastic_strain_increment,
                           const RankTwoTensor & rotation_increment,
                           RankTwoTensor & stress_new,
                           const RankTwoTensor & stress_old,
                           const RankFourTensor & elasticity_tensor,
                           const RankTwoTensor & elastic_strain_old,
                           bool compute_full_tangent_operator,
                           RankFourTensor & tangent_operator) override;

protected:
  virtual void initQpStatefulProperties() override;
  virtual void propagateQpStatefulProperties() override;

  
  virtual Real initialGuess(const Real /*effective_trial_stress*/) { return 1.0; }

  virtual Real minimumPermissibleValue(const Real) const override;

  
  virtual void computeStressInitialize(const Real effective_trial_stress,
                                       const RankFourTensor & elasticity_tensor) override;
  
  virtual TangentCalculationMethod getTangentCalculationMethod() override
  {
    return TangentCalculationMethod::PARTIAL;
  }
  
//  virtual Real computeStressDerivative(const Real effective_trial_stress, const Real scalar);

  virtual Real computeResidual(const Real effective_trial_stress, const Real scalar) override;
  virtual Real computeDerivative(const Real effective_trial_stress, const Real scalar) override;
  virtual void iterationFinalize(Real scalar) override;
  virtual void computeStressFinalize(const RankTwoTensor & plasticStrainIncrement) override;

  virtual Real computeReferenceResidual(const Real effective_trial_stress, 
                        const Real scalar_effective_inelastic_strain) override;
  
  virtual void computeYieldStress(const RankFourTensor & elasticity_tensor);
  virtual Real computeHardeningValue(Real scalar);
  virtual Real computeHardeningDerivative(Real scalar);
  
  
 
  /// a string to prepend to the plastic strain Material Property name
  const std::string _plastic_prepend;

  Real _mrate; // rate_exponent
  Real _epsilon_rate; // reference_strain_rate
  Real _Y0; // initial_resistance
  Real _H0; // Hardening modulus
  Real _Ysat; // Saturation value
  Real _ahard; // Hardening exponent
  
  // Equivalent shear properties 
  Real _shear_rate;
  Real _shear_initial_resistance;
  Real _shear_saturation;
  Real _shear_initial_hardness;

  /// plastic strain in this model
  MaterialProperty<RankTwoTensor> & _plastic_strain;

  /// old value of plastic strain
  const MaterialProperty<RankTwoTensor> & _plastic_strain_old;

  
  MaterialProperty<Real> & _yield_strength;
  
  MaterialProperty<Real> & _hardening_variable;
  const MaterialProperty<Real> & _hardening_variable_old;

  MaterialProperty<Real> & _strength_variable;
  const MaterialProperty<Real> & _strength_variable_old;

  const MaterialProperty<RankTwoTensor> & _deformation_gradient;
  const MaterialProperty<RankTwoTensor> & _deformation_gradient_old;
  
  MaterialProperty<RankTwoTensor> & _Fp;
  const MaterialProperty<RankTwoTensor> & _Fp_old;
};

#endif /* ISOTROPICHYPERVISCOSTRESSUPDATE_H */

