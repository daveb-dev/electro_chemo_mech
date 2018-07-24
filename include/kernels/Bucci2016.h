/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Bucci2016.h
 * Author: srinath
 *
 * Created on July 24, 2018, 9:48 AM
 */

#ifndef BUCCI2016_H
#define BUCCI2016_H

#include "ALEKernel.h"
#include "RankTwoTensor.h"
#include "RankFourTensor.h"

//Forward Declaration
class Bucci2016;
class RankTwoTensor;
class RankFourTensor;

template<>
InputParameters validParams<Bucci2016>();

class Bucci2016 : public ALEKernel
{
public:
  Bucci2016(const InputParameters & parameters);
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
  
//  const bool _out_of_plane_strain_coupled;
//  const unsigned int _out_of_plane_strain_var;
  const unsigned int _out_of_plane_direction;


  /// Gradient of test function averaged over the element. Used in volumetric locking correction calculation.
  std::vector<std::vector<Real>> _avg_grad_test;

  /// Gradient of phi function averaged over the element. Used in volumetric locking correction calculation.
  std::vector<std::vector<Real>> _avg_grad_phi;

  /// Flag for volumetric locking correction
  bool _volumetric_locking_correction;
  
  /// Flag for geometric jacobian contribution
  bool _use_geometric_jacobian;
  
  
private:

};

#endif /* BUCCI2016_H */

