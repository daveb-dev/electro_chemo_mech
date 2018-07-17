/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   StressDivergenceConcentrationTensor.h
 * Author: srinath
 *
 * Created on July 9, 2018, 1:51 PM
 */

#ifndef STRESSDIVERGENCECONCENTRATIONTENSOR_H
#define STRESSDIVERGENCECONCENTRATIONTENSOR_H

#include "ALEKernel.h"
#include "RankTwoTensor.h"
#include "RankFourTensor.h"

//Forward Declaration
class StressDivergenceConcentrationTensor;
class RankTwoTensor;
class RankFourTensor;

template<>
InputParameters validParams<StressDivergenceConcentrationTensor>();

class StressDivergenceConcentrationTensor : public ALEKernel
{
public:
  StressDivergenceConcentrationTensor(const InputParameters & parameters);
  virtual void computeJacobian() override;
  virtual void computeOffDiagJacobian(MooseVariableFEBase & jvar) override;
  using Kernel::computeOffDiagJacobian;    
protected:
  virtual void initialSetup() override;

  virtual void computeResidual() override;
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
  virtual Real computeQpOffDiagJacobian(unsigned int jvar) override;

  //virtual void computeFiniteDeformJacobian();
  virtual void computeAverageGradientTest();
  virtual void computeAverageGradientPhi();

  std::string _base_name;
  //bool _use_finite_deform_jacobian;

  const MaterialProperty<RankTwoTensor> & _stress;
  const MaterialProperty<RankFourTensor> & _Jacobian_mult;

  const MaterialProperty<RankTwoTensor> &_deformation_gradient;

  const unsigned int _component;

  /// Coupled displacement variables
  unsigned int _ndisp;
  std::vector<unsigned int> _disp_var;

  /// Gradient of test function averaged over the element. Used in volumetric locking correction calculation.
  std::vector<std::vector<Real>> _avg_grad_test;

  /// Gradient of phi function averaged over the element. Used in volumetric locking correction calculation.
  std::vector<std::vector<Real>> _avg_grad_phi;

  /// Flag for volumetric locking correction
  bool _volumetric_locking_correction;
  
  
private:

};

#endif /* STRESSDIVERGENCECONCENTRATIONTENSOR_H */

