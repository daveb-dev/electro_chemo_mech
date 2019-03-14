/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ComputeHyperViscoIsoStress.h
 * Author: srinath
 *
 * Created on March 14, 2019, 3:24 PM
 */

#ifndef COMPUTEHYPERVISCOISOSTRESS_H
#define COMPUTEHYPERVISCOISOSTRESS_H

#include "ComputeFiniteStrainElasticStress.h"

#include "StressUpdateBase.h"

class ComputeHyperViscoIsoStress;

template <>
InputParameters validParams<ComputeHyperViscoIsoStress>();

/**
 * ComputeHyperViscoIsoStress computes the stress, the consistent tangent
 * operator (or an approximation to it). By default finite strains are assumed.
 *
 * The formulation Weber and Anand (1982) is used and the deformation gradient
 * decomposition into F_e and F_p is used to compute the stress. The computed
 * stress is the Mandel Stress which is work conjugate to the logarithmic strain. 
 * Stateful variables coming out of this are 
 *   a) Plastic Distortion (F_p)
 *   b) Equivalent plastic shear strain rate
 * TODO: Isotropic expansion is considered and then anisotropy is introduced 
 * into the expansion term. 
 *
 * This material is used to call the recompute iterative materials of a number
 * of specified inelastic models that inherit from StressUpdateBase.  It iterates
 * over the specified inelastic models until the change in stress is within
 * a user-specified tolerance, in order to produce the stress, the consistent
 * tangent operator and the elastic and inelastic strains for the time increment.
 */

class ComputeHyperViscoIsoStress : public ComputeFiniteStrainElasticStress
{
public:
  ComputeHyperViscoIsoStress(const InputParameters & parameters);

  virtual void initialSetup() override;

protected:
  virtual void initQpStatefulProperties() override;

  virtual void computeQpStress() override;

  /**
    Compute the stress for the current QP, but do not rotate tensors from the
    intermediate configuration to the new configuration
   */
  virtual void computeQpStressIntermediateConfiguration();

  /**
    Rotate _elastic_strain, _stress, _inelastic_strain, and _Jacobian_mult to the
    new configuration.
    @param force_elasticity_rotation Force the elasticity tensor to be rotated, even if
    it is not deemed necessary.
   */
  virtual void finiteStrainRotation(const bool force_elasticity_rotation = false);


  /**
   */
  virtual void updateQpStateSingleModel(unsigned model_number,
                                        RankTwoTensor & elastic_strain_increment,
                                        RankTwoTensor & combined_inelastic_strain_increment);

  /**
   * Using _elasticity_tensor[_qp] and the consistent tangent operators,
   * _consistent_tangent_operator[...] computed by the inelastic models,
   * compute _Jacobian_mult[_qp]
   */
  virtual void computeQpJacobianMult();

  /**
   * Given a trial stress (_stress[_qp]) and a strain increment (elastic_strain_increment)
   * let the model_number model produce an admissible stress (gets placed back
   * in _stress[_qp]), and decompose the strain increment into an elastic part
   * (gets placed back into elastic_strain_increment) and an
   * inelastic part (inelastic_strain_increment), as well as computing the
   * consistent_tangent_operator
   * @param model_number The inelastic model to use
   * @param elastic_strain_increment Upon input, this is the strain increment.
   * Upon output, it is the elastic part of the strain increment
   * @param inelastic_strain_increment The inelastic strain increment
   * corresponding to the supplied strain increment
   * @param consistent_tangent_operator The consistent tangent operator
   */
  virtual void computeAdmissibleState(unsigned model_number,
                                      RankTwoTensor & elastic_strain_increment,
                                      RankTwoTensor & inelastic_strain_increment,
                                      RankFourTensor & consistent_tangent_operator);

  ///@{Input parameters associated with the recompute iteration to return the stress state to the yield surface
  const unsigned int _max_iterations;
  const Real _relative_tolerance;
  const Real _absolute_tolerance;
  const bool _internal_solve_full_iteration_history;
  ///@}

  /// after updateQpState, rotate the stress, elastic_strain, inelastic_strain and Jacobian_mult using _rotation_increment
  const bool _perform_finite_strain_rotations;

  ///@{ Strain tensors
  const MaterialProperty<RankTwoTensor> & _elastic_strain_old;
  const MaterialProperty<RankTwoTensor> & _strain_increment;
  ///@}

  /// The sum of the inelastic strains that come from the plastic models
  MaterialProperty<RankTwoTensor> & _inelastic_strain;

  /// old value of inelastic strain
  const MaterialProperty<RankTwoTensor> & _inelastic_strain_old;

  /// what sort of Tangent operator to calculate
  const enum class TangentOperatorEnum { elastic, nonlinear } _tangent_operator_type;

  /// number of plastic models
  const unsigned _num_models;

  /// Flags to compute tangent during updateState call
  std::vector<bool> _tangent_computation_flag;

  /// Calculation method for the tangent modulus
  TangentCalculationMethod _tangent_calculation_method;

  /// _inelastic_strain = sum_i (_inelastic_weights_i * inelastic_strain_from_model_i)
  const std::vector<Real> _inelastic_weights;

  /// the consistent tangent operators computed by each plastic model
  std::vector<RankFourTensor> _consistent_tangent_operator;

  /// whether to cycle through the models, using only one model per timestep
  const bool _cycle_models;

  MaterialProperty<Real> & _matl_timestep_limit;

  /**
   * Rank four symmetric identity tensor
   */
  const RankFourTensor _identity_symmetric_four;

  /**
   * The user supplied list of inelastic models to use in the simulation
   *
   * Users should take care to list creep models first and plasticity
   * models last to allow for the case when a creep model relaxes the stress state
   * inside of the yield surface in an iteration.
   */
  std::vector<StressUpdateBase *> _models;

  /// is the elasticity tensor guaranteed to be isotropic?
  bool _is_elasticity_tensor_guaranteed_isotropic;
};

#endif /* COMPUTEHYPERVISCOISOSTRESS_H */

