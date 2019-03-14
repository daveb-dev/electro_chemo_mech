/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   StressDivergenceConcentrationTensors.h
 * Author: srinath
 *
 * Created on September 10, 2018, 3:02 PM
 */

#ifndef STRESSDIVERGENCECONCENTRATIONTENSORS_H
#define STRESSDIVERGENCECONCENTRATIONTENSORS_H

#include "ALEKernel.h"
#include "RankTwoTensor.h"
#include "RankFourTensor.h"

// Forward Declarations
class StressDivergenceConcentrationTensors;
//class RankTwoTensor;
//class RankFourTensor;

template <>
InputParameters validParams<StressDivergenceConcentrationTensors>();

/**
 * StressDivergenceConcentrationTensors mostly copies from StressDivergence.  There are small changes to use
 * RankFourTensor and RankTwoTensors instead of SymmElasticityTensors and SymmTensors.  This is done
 * to allow for more mathematical transparancy.
 */
class StressDivergenceConcentrationTensors : public ALEKernel
{
public:
  StressDivergenceConcentrationTensors(const InputParameters & parameters);

  virtual void computeJacobian() override;
  virtual void computeOffDiagJacobian(MooseVariableFEBase & jvar) override;
  using Kernel::computeOffDiagJacobian;

protected:
  virtual void initialSetup() override;

  virtual void computeResidual() override;
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
  virtual Real computeQpOffDiagJacobian(unsigned int jvar) override;

  virtual void computeFiniteDeformJacobian();
//  virtual void computeDeformedTest();
//  virtual void computeDeformedPhi();
  
  virtual void computeAverageGradientTest();
  virtual void computeAverageGradientPhi();
  virtual void computeAverageVolumeChange();
  

  std::string _base_name;
  bool _use_finite_deform_jacobian;

  const MaterialProperty<RankTwoTensor> & _stress;
  const MaterialProperty<RankFourTensor> & _Jacobian_mult;

  std::vector<RankFourTensor> _finite_deform_Jacobian_mult;
  const MaterialProperty<RankTwoTensor> * _deformation_gradient;
  const MaterialProperty<RankTwoTensor> * _deformation_gradient_old;
  const MaterialProperty<RankTwoTensor> * _rotation_increment;
  // MaterialProperty<RankTwoTensor> & _d_stress_dT;

  const unsigned int _component;

  /// Coupled displacement variables
  unsigned int _ndisp;
  std::vector<unsigned int> _disp_var;

  const bool _temp_coupled;
  const unsigned int _temp_var;

  /// d(strain)/d(temperature), if computed by ComputeThermalExpansionEigenstrain
  const MaterialProperty<RankTwoTensor> * const _deigenstrain_dT;

  const bool _conc_coupled;
  const unsigned int _conc_var;

  /// d(strain)/d(concentration), if computed by ComputeConcentrationEigenstrain
  const MaterialProperty<RankTwoTensor> * const _deigenstrain_dC;


  const bool _out_of_plane_strain_coupled;
  const unsigned int _out_of_plane_strain_var;
  const unsigned int _out_of_plane_direction;

  std::vector<std::vector<Real>> _grad_test_deformed;
  
  std::vector<std::vector<Real>> _grad_phi_deformed;
  
  /// Gradient of test function averaged over the element. Used in volumetric locking correction calculation.
  std::vector<std::vector<Real>> _avg_grad_test;

  /// Gradient of phi function averaged over the element. Used in volumetric locking correction calculation.
  std::vector<std::vector<Real>> _avg_grad_phi;
  
  Real _avg_vol_change;
  
  /// Flag for volumetric locking correction
  bool _volumetric_locking_correction;
};

#endif // STRESSDIVERGENCECONCENTRATIONTENSORS_H
