/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   HEVAnandHardening.h
 * Author: srinath
 *
 * Created on November 1, 2018, 12:55 PM
 */

#ifndef HEVANANDHARDENING_H
#define HEVANANDHARDENING_H

#include "HEVPStrengthUOBase.h"

class HEVAnandHardening;

template <>
InputParameters validParams<HEVAnandHardening>();

/**
 * This user object classs
 * Computes power law  hardening using rate form
 * See Narayan and Anand Extreme Mechanics Letters 2018,
 */


class HEVAnandHardening : public HEVPStrengthUOBase {
public:
  HEVAnandHardening(const InputParameters & parameters);

  virtual bool computeValue(unsigned int, Real, Real &) const;
  virtual bool computeDerivative(unsigned int, const std::string &, Real &) const;

protected:
    Real _H0;
    Real _Ssat;
    Real _alpha;
    Real _S0;
private:

};

#endif /* HEVANANDHARDENING_H */

