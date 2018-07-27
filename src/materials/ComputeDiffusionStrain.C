/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ComputeDiffusionStrain.C
 * Author: srinath
 * 
 * Created on July 26, 2018, 9:59 AM
 */

#include "ComputeDiffusionStrain.h"
#include "MooseMesh.h"
#include "Assembly.h"
#include "libmesh/quadrature.h"

registerMooseObject("electro_chemo_mechApp", ComputeDiffusionStrain);

template<>
InputParameters 
validParams<ComputeDiffusionStrain>()
{
    InputParameters params = validParams<Material>();
    params.addRequiredCoupledVar("concentration","Concentration variable") ;
    return params;
}

ComputeDiffusionStrain::ComputeDiffusionStrain(const InputParameters& parameters)
        : DerivativeMaterialInterface<Material>(parameters),
        _conc(coupledValue("concentration")),
        _deformation_gradient_diffusion(declareProperty<RankTwoTensor>("deformation_gradient_diffusion")),
        _dFdc(declareProperty<RankTwoTensor>("dFdc")),
        _mobility(getMaterialPropertyByName<Real>("mobility")),
        _lattice_misfit(getMaterialPropertyByName<Real>("lattice_misfit")),
        _molar_volume(getMaterialPropertyByName<Real>("molar_volume"))
{
    
}        

void 
ComputeDiffusionStrain::initialSetup()
{
    _deformation_gradient_diffusion[_qp].zero();
    _dFdc[_qp].zero();
    _deformation_gradient_diffusion[_qp].addIa(1.0);
}

void
ComputeDiffusionStrain::computeProperties()
{
    for (_qp =0; _qp < _qrule->n_points(); ++_qp)
    {
        _deformation_gradient_diffusion[_qp].zero();
        _deformation_gradient_diffusion[_qp].addIa(1.0);
        _deformation_gradient_diffusion[_qp].addIa(_conc[_qp]*_lattice_misfit[_qp]/_molar_volume[_qp]);
        _dFdc[_qp].addIa(_lattice_misfit[_qp]/_molar_volume[_qp]);
    }
}