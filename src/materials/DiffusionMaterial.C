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

#include "libmesh/quadrature.h"

registerMooseObject("electro_chemo_mechApp", DiffusionMaterial);

template <>
InputParameters
validParams<DiffusionMaterial>()
{
    InputParameters params = validParams<Material>();
    params.addRequiredParam<Real>("mobility","Diffusion coeffcient of intercalating species");
    params.addRequiredParam<Real>("lattice_misfit", "Lattice misfit of intercalating species (beta)");
    params.addRequiredParam<Real>("molar_volume","Molar volume of host (rho_h)");
    params.addRequiredParam<Real>("max_concentration", "Maximum Conctration of species");
    return params;
}

DiffusionMaterial::DiffusionMaterial(const InputParameters& parameters) 
: Material(parameters),
  _M(getParam<Real>("mobility")),
  _beta(getParam<Real>("lattice_misfit")),
  _rhoh(getParam<Real>("molar_volume")),
  _cmax(getParam<Real>("max_concentration")),
  _mobility(declareProperty<Real>("mobility")),
  _lattice_misfit(declareProperty<Real>("lattice_misfit")),
  _molar_volume(declareProperty<Real>("molar_volume")),
  _max_concentration(declareProperty<Real>("max_concentration"))
{
}

void
DiffusionMaterial::initialSetup()
{
  _mobility[_qp] = _M;
  _lattice_misfit[_qp] = _beta;
  _molar_volume[_qp] = _rhoh;
  _max_concentration[_qp] = _cmax;
}

void
DiffusionMaterial::computeProerties()
{
    _mobility[_qp] = _M;
    _lattice_misfit[_qp] = _beta;
    _molar_volume[_qp] = _rhoh;
    _max_concentration[_qp] = _cmax;
 }

