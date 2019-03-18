/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IsotropicHyperViscoStressUpdate.C
 * Author: srinath
 * 
 * Created on March 18, 2019, 8:27 AM
 */

#include "IsotropicHyperViscoStressUpdate.h"

#include "Function.h"
#include "ElasticityTensorTools.h"

registerMooseObject("electro_chemo_mechApp", IstotropicHyperViscoStressUpdate);

template <>
InputParameters
validParams<IsotropicPlasticityStressUpdate>()
{
  InputParameters params = validParams<RadialReturnStressUpdate>();
  params.addClassDescription("This class uses the discrete material in a radial return isotropic "
                             "plasticity model.  This class is one of the basic radial return "
                             "constitutive models, yet it can be used in conjunction with other "
                             "creep and plasticity materials for more complex simulations.");
  
  params.addParam<Real>("rate_exponent", 50.0, "Rate Sensitivity parameter");
  params.addParam<Real>("reference_strain_rate",0.001, "Reference strain rate");
  params.addParam<Real>("initial_resistance", 0.0, "Initial resistance in tension");
  params.addParam<Real>("hardening_modulus", 1.0, "Hardening modulus in tension");
  params.addParam<Real>("saturation_resistance", 1.0, "Saturation value of hardening in tension");
  params.addParam<Real>("hardening_exponent", 1.0, "Hardening exponent");
  params.addDeprecatedParam<std::string>(
      "plastic_prepend",
      "",
      "String that is prepended to the plastic_strain Material Property",
      "This has been replaced by the 'base_name' parameter");
  params.set<std::string>("effective_inelastic_strain_name") = "effective_plastic_strain";

  return params;
}

IsotropicPlasticityStressUpdate::IsotropicPlasticityStressUpdate(const InputParameters & parameters)
  : RadialReturnStressUpdate(parameters),
    _plastic_prepend(getParam<std::string>("plastic_prepend")),
    _mrate(getParam<Real>("rate_exponent")),
    _epsilon_rate(getParam<Real>("reference_strain_rate")),
    _Y0(getParam<Real>("initial_resistance")),
    _H0(getParam<Real>("hardening_modulus")),
    _Ysat(getParam<Real>("saturation_resistance")),
    _ahard(getParam<Real>("hardening_exponent")),
    _plastic_strain(
        declareProperty<RankTwoTensor>(_base_name + _plastic_prepend + "plastic_strain")),
    _plastic_strain_old(
        getMaterialPropertyOld<RankTwoTensor>(_base_name + _plastic_prepend + "plastic_strain")),
    _hardening_variable(declareProperty<Real>(_base_name + "hardening_variable")),
    _hardening_variable_old(getMaterialPropertyOld<Real>(_base_name + "hardening_variable")), 
    _Fp(declarePrporety<RankTwoTensor>(_base_name + _plastic_prepend + "plastic_distortion")),        
    _Fp_old(getMaterialPropertyOld<RankTwoTensor>(_base_name + _plastic_prepend + "plastic_distortion"))
{
}

void
IsotropicPlasticityStressUpdate::initQpStatefulProperties()
{
  _hardening_variable[_qp] = 0.0;
  _plastic_strain[_qp].zero();
  _plastic_distortion[_qp].zero();
}

void
IsotropicPlasticityStressUpdate::propagateQpStatefulProperties()
{
  _hardening_variable[_qp] = _hardening_variable_old[_qp];
  _plastic_strain[_qp] = _plastic_strain_old[_qp];

  propagateQpStatefulPropertiesRadialReturn();
}

void
IsotropicPlasticityStressUpdate::computeStressInitialize(const Real effective_trial_stress,
                                                         const RankFourTensor & elasticity_tensor)
{
  computeYieldStress(elasticity_tensor);

  _yield_condition = effective_trial_stress - _hardening_variable_old[_qp] - _yield_stress;
  _hardening_variable[_qp] = _hardening_variable_old[_qp];
  _plastic_strain[_qp] = _plastic_strain_old[_qp];
}

Real
IsotropicPlasticityStressUpdate::computeResidual(const Real effective_trial_stress,
                                                 const Real scalar)
{
  Real residual = 0.0;

  mooseAssert(_yield_condition != -1.0,
              "the yield stress was not updated by computeStressInitialize");

  if (_yield_condition > 0.0)
  {
    _hardening_slope = computeHardeningDerivative(scalar);
    _hardening_variable[_qp] = computeHardeningValue(scalar);

    residual =
        (effective_trial_stress - _hardening_variable[_qp] - _yield_stress) / _three_shear_modulus -
        scalar;
  }
  return residual;
}

Real
IsotropicPlasticityStressUpdate::computeDerivative(const Real /*effective_trial_stress*/,
                                                   const Real /*scalar*/)
{
  Real derivative = 1.0;
  if (_yield_condition > 0.0)
    derivative = -1.0 - _hardening_slope / _three_shear_modulus;

  return derivative;
}

void
IsotropicPlasticityStressUpdate::iterationFinalize(Real scalar)
{
  if (_yield_condition > 0.0)
    _hardening_variable[_qp] = computeHardeningValue(scalar);
}

void
IsotropicPlasticityStressUpdate::computeStressFinalize(const RankTwoTensor & plasticStrainIncrement)
{
  _plastic_strain[_qp] += plasticStrainIncrement;
}

Real
IsotropicPlasticityStressUpdate::computeHardeningValue(Real scalar)
{
  Real value = _hardening_variable_old[_qp] + (_hardening_slope * scalar);
  if (_hardening_function)
  {
    const Real strain_old = _effective_inelastic_strain_old[_qp];
    Point p;

    value = _hardening_function->value(strain_old + scalar, p) - _yield_stress;
  }
  return value;
}

Real IsotropicPlasticityStressUpdate::computeHardeningDerivative(Real /*scalar*/)
{
  Real slope = _hardening_constant;
  if (_hardening_function)
  {
    const Real strain_old = _effective_inelastic_strain_old[_qp];
    Point p; // Always (0,0,0)

    slope = _hardening_function->timeDerivative(strain_old, p);
  }
  return slope;
}

void
IsotropicPlasticityStressUpdate::computeYieldStress(const RankFourTensor & /*elasticity_tensor*/)
{
  if (_yield_stress_function)
  {
    Point p;
    _yield_stress = _yield_stress_function->value(_temperature[_qp], p);
    if (_yield_stress <= 0.0)
      mooseError("The yield stress must be greater than zero, but during the simulation your yield "
                 "stress became less than zero.");
  }
}
