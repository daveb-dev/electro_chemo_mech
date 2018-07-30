/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Bower2012.h
 * Author: srinath
 *
 * Created on July 24, 2018, 9:48 AM
 */

#ifndef BOWER2012_H
#define BOWER2012_H

#include "ALEKernel.h"
#include "RankTwoTensor.h"
#include "RankFourTensor.h"

//Forward Declaration
class Bower2012;
class RankTwoTensor;
class RankFourTensor;

template<>
InputParameters validParams<Bower2012>();

class Bower2012 : public ALEKernel
{
public:
  Bower2012(const InputParameters & parameters);
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

  const MaterialProperty<RankTwoTensor> &_deformation_gradient;

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

