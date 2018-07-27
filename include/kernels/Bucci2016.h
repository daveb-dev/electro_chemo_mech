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


  std::string _base_name;

  const MaterialProperty<RankTwoTensor> & _stress;
  const MaterialProperty<RankFourTensor> & _Jacobian_mult;

  const MaterialProperty<RankTwoTensor> & _deformation_gradient;
  const MaterialProperty<RankTwoTensor> & _deformation_gradient_diffusion;
  const MaterialProperty<RankTwoTensor> & _dFdc;
  

  const unsigned int _component;

  /// Coupled displacement variables
  unsigned int _ndisp;
  std::vector<unsigned int> _disp_var;
  
  const bool _conc_coupled;
  const unsigned int _conc_var;
  
  const unsigned int _out_of_plane_direction;
   
  
private:

};

#endif /* BUCCI2016_H */

