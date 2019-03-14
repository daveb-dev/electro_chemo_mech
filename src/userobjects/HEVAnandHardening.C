/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   HEVAnandHardening.C
 * Author: Srinath Chakravarthy
 * 
 * Created on November 1, 2018, 12:55 PM
 */

#include "HEVAnandHardening.h"

registerMooseObject("electro_chemo_mechApp", HEVAnandHardening);

template <>
InputParameters
validParams<HEVAnandHardening>()
{
  InputParameters params = validParams<HEVPStrengthUOBase>();
  params.addRequiredParam<Real>("hardening_modulus", "Hardening Modulus");
  params.addRequiredParam<Real>("saturation_hardening", "Saturation value of hardening");
  params.addRequiredParam<Real>("hardening_exponent", "The hardening exponent value");
  params.addRequiredParam<Real>("initial_yield_stress", "Initial value of yield stress");
  params.addRequiredParam<std::string>("intvar_rate_prop_name",
                               "Name of internal variable property to "
                               "calculate material resistance: Same as "
                               "internal variable user object");
  params.addClassDescription("User object for Ramberg-Osgood hardening power law hardening");

  return params;
}

HEVAnandHardening::HEVAnandHardening(const InputParameters & parameters)
  : HEVPStrengthUOBase(parameters),
    _H0(getParam<Real>("hardening_modulus")),
    _Ssat(getParam<Real>("saturation_hardening")),
    _alpha(getParam<Real>("hardening_exponent")),
    _S0(getParam<Real>("initial_yield_stress")),
    _intvar_rate_prop_name(getParam<std::string>("intvar_rate_prop_name")),
    _intvar_rate(getMaterialPropertyByName<Real>(_intvar_rate_prop_name)),        
    _this_old(getMaterialPropertyOldByName<Real>(_name))
        
{   

}

Real 
HEVAnandHardening::initVal() const
{
    return _S0;
}
bool
HEVAnandHardening::computeValue(unsigned int qp, Real dt, Real & val) const
{
    Real Ht, St, nup_t;
//    if (_t_step < 1) {
//        St = _S0;
//        nup_t = 0.5;
//    } else { 
////        if (_this_old[qp] < _S0)
////        {
////            St = _S0;
////        } else
////        {
//            St = _this_old[qp];
////        }
//        nup_t = _intvar_rate[qp];
//    }
    nup_t = _intvar_rate[qp];
    St = _this_old[qp];
    Ht = _H0 * std::pow(1.0 - St / _Ssat, _alpha);
    val = St  + dt*Ht*nup_t;
  return true;
}

bool
HEVAnandHardening::computeDerivative(unsigned int qp,
                                              Real dt,
                                              const std::string & coupled_var_name,
                                              Real & val) const
{
  val = 0;
  
  if (_intvar_prop_name == coupled_var_name) {
      if (_t_step == 1){
          val = _H0*std::pow(1.0 - _S0/_Ssat, _alpha);
      } else {
          val = _H0*std::pow(1.0 - _this_old[qp]/_Ssat, _alpha);
      }
//      val *= std::sqrt(3.0);
  }

  return true;
}