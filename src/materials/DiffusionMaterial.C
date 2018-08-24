/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DiffusionMaterial.C
 * Author: srinath
 * 
 * Created on July 25, 2018, 10:06 AM
 */

#include "DiffusionMaterial.h"
#include "Function.h"

#include "libmesh/quadrature.h"

registerMooseObject("electro_chemo_mechApp", DiffusionMaterial);

template <>
InputParameters
validParams<DiffusionMaterial>()
{
    InputParameters params = validParams<Material>();
    params.addCoupledVar("concentration", "Coupled Concentration");
    
    params.addRequiredParam<Real>("mobility","Diffusion coeffcient of intercalating species");
    params.addParam<FunctionName>("mobility_concentration_function",
                                  "",
                                  "Diffusion coefficient as a function of concentration");
    params.addParam<Real>("activity_coefficient", "Activity coefficient in chemical potenttial");
    params.addParam<FunctionName>("activity_coefficient_concentration_function",
                                   "",
                                   "Activity Coefficient as a function of concentration");
    
    params.addParam<Real>("lattice_misfit", "Lattice misfit of intercalating species (beta)");
    params.addParam<Real>("molar_volume","Molar volume of host (rho_h)");
    params.addParam<Real>("max_concentration", "Maximum Concentration of species");
    params.addParam<Real>("gas_constant", "Universal Gas Constant");
    params.addParam<Real>("temperature", "Temperature");
    params.addClassDescription("General-purpose material model for chemo diffusion");
    return params;
}

DiffusionMaterial::DiffusionMaterial(const InputParameters& parameters) 
: Material(parameters),
  _has_conc(isCoupled("concentration")),
  _concentration(_has_conc ? coupledValue("concentration") : _zero),  
  _M(isParamValid("mobility") ? getParam<Real>("mobility") : 0),
  _gamma(isParamValid("activity_coefficient") ? getParam<Real>("activity_coefficient") : 1.0),  
  _beta(isParamValid("lattice_misfit") ? getParam<Real>("lattice_misfit") : 0),
  _rhoh(isParamValid("molar_volume") ? getParam<Real>("molar_volume") : 0),
  _cmax(isParamValid("max_concentration") ? getParam<Real>("max_concentration") : 1.0), 
  _R(isParamValid("gas_constant") ? getParam<Real>("gas_constant") : 1.0),
  _temp(isParamValid("temperature") ? getParam<Real>("temperature") : 1.0),
  _mobility(declareProperty<Real>("mobility")),
  _mobility_dC(declareProperty<Real>("mobility_dC")),
  _mobility_concentration_function(
        getParam<FunctionName>("mobility_concentration_function") != ""
            ? &getFunction("mobility_concentration_function")
            : NULL),
  _activity_coefficient(declareProperty<Real>("activity_coefficient")),
  _activity_coefficient_dC(declareProperty<Real>("activity_coefficient_dC")),
  _actitivity_coefficient_concentration_function(
            getParam<FunctionName>("activity_coefficient_concentration_function") != ""
            ?  &getFunction("activity_coefficient_concentration_function")
            : NULL),
  _lattice_misfit(declareProperty<Real>("lattice_misfit")),
  _molar_volume(declareProperty<Real>("molar_volume")),
  _max_concentration(declareProperty<Real>("max_concentration"))
{
    if (_mobility_concentration_function && !_has_conc)
    {
        mooseError("Must couple with concentration if using diffusivity function");
    }
    if (isParamValid("mobility") && _mobility_concentration_function)
    {
        mooseError(
        "Cannot define both diffusion coefficient and diffusivity concentration function");
    }
    if (_actitivity_coefficient_concentration_function && !_has_conc)
    {
        mooseError("Must couple with concentration if using activity_coefficient function");
    }
    if (isParamValid("activity_coefficient") && _actitivity_coefficient_concentration_function)
    {
        mooseError(
        "Cannot define both activity_coefficient and activity_coefficient_concenttration_function");
    }
    
}


void
DiffusionMaterial::computeProperties()
{
  for (unsigned int qp(0); qp < _qrule->n_points(); ++qp)
  {
    Real qp_concentration = 0;
    if (_has_conc)
    {
      qp_concentration = _concentration[qp];
//      if (_concentration[qp] < 0)
//      {
//        std::stringstream msg;
//        msg << "WARNING:  In HeatConductionMaterial:  negative concentration!\n"
//            << "\tResetting to zero.\n"
//            << "\t_qp: " << qp << "\n"
//            << "\ttemp: " << _concentration[qp] << "\n"
//            << "\telem: " << _current_elem->id() << "\n"
//            << "\tproc: " << processor_id() << "\n";
//        mooseWarning(msg.str());
//        qp_concentration = 0;
//      }
    }
    if (_mobility_concentration_function)
    {
      Point p;
      _mobility[qp] =
          _mobility_concentration_function->value(qp_concentration, p)/(_R *_temp);
      _mobility_dC[qp] =
          _mobility_concentration_function->timeDerivative(qp_concentration, p)/(_R *_temp);
    }
    else
    {
      _mobility[qp] = _M/(_R * _temp); // Actually Represents diffusion coefficient
      _mobility_dC[qp] = 0;
    }

    if ( _actitivity_coefficient_concentration_function)
    {
      Point p;
      _activity_coefficient[qp] =  _actitivity_coefficient_concentration_function->value(qp_concentration, p);
    }
    else
    {
      _activity_coefficient[qp] = _gamma;
    }
  }
    
}

