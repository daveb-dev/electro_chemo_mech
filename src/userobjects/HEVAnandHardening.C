/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   HEVAnandHardening.C
 * Author: srinath
 * 
 * Created on November 1, 2018, 12:55 PM
 */

#include "HEVAnandHardening.h"

registerMooseObject("TensorMechanicsApp", HEVPAnandHardening);

template <>
InputParameters
validParams<HEVAnandHardening>()
{
  InputParameters params = validParams<HEVPStrengthUOBase>();
  params.addParam<Real>("hardening_modulus", "Hardening Modulus");
  params.addRequiredParam<Real>("saturation_hardening", "Saturation value of hardening");
  params.addRequiredParam<Real>("hardening_exponent", "The hardening exponent value");
  params.addRequiredParam<Real>("initial_yield_stress", "Initial value of yield stress");
  params.addClassDescription("User object for Ramberg-Osgood hardening power law hardening");

  return params;
}

HEVAnandHardening::HEVAnandHardening(const InputParameters & parameters)
  : HEVPStrengthUOBase(parameters),
    _H0(getParam<Real>("hardening_modulus")),
    _Ssat(getParam<Real>("saturation_hardening")),
    _alpha(getParam<Real>("hardening_exponent")),
    _S0(getParam<Real>("initial_yield_stress"))
{
}

bool
HEVAnandHardening::computeValue(unsigned int qp, Real dt, Real & val) const
{
    if (_t == 0) {
        val = _H0;
    } else {
//        Real Ht = _H0*(1.0 - _this_old[_qp]/_Ssat)
        val = _this_old[_qp] + dt * _intvar[qp];
    }
  return true;
}

bool
HEVAnandHardening::computeDerivative(unsigned int qp,
                                              const std::string & coupled_var_name,
                                              Real & val) const
{
  val = 0;

  if (_intvar_prop_name == coupled_var_name) {
      val = _H0*std::pow(1.0 - _this_old[_qp]/_Ssat, _alpha);
  }

  return true;
}