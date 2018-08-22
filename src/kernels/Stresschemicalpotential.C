/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Stresschemicalpotential.C
 * Author: srinath
 * 
 * Created on August 22, 2018, 3:18 PM
 */

#include "Stresschemicalpotential.h"
#include "ElasticityTensorTools.h"
#include "Material.h"
#include "MooseMesh.h"
#include "MooseVariable.h"
#include "SystemBase.h"

#include "libmesh/quadrature.h"

registerMooseObject("electro_chemo_mechApp", Stresschemicalpotential);

template<>
InputParameters
validParams<Stresschemicalpotential>()
{
    InputParameters params = validParams<Kernel>();
    params.addClassDescription("Solves stress based chemical potential");

    params.addRequiredCoupledVar("chemical_potential", "name of chemical_potential");
    params.addRequiredParam<unsigned int>("component",
                                        "An integer corresponding to the direction "
                                        "the variable this kernel acts in. (0 for x, "
                                        "1 for y, 2 for z)");
    params.addRequiredCoupledVar("displacements",
                               "The string of displacements suitable for the problem statement");
    params.addCoupledVar("concentration",
                        "The name of the concentration variable used in"
                        "ComputeConcentrationEigenstrain");
    params.addParam<std::string>(
      "concentration_eigenstrain_name",
      "concentration_eigenstrain",
      "Eigenstrain name used in ComputeConcentrationEigenstrain");
    return params;
}

Stresschemicalpotential::Stresschemicalpotential(const InputParameters & parameters)
        : DerivativeMaterialInterface<Kernel>(parameters),
        _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
        _grad_chemical_potential(coupledGradient("chemical_potential")),
        _stress(getMaterialPropertyByName<RankTwoTensor>(_base_name + "stress")),
        _stress_old(getMaterialPropertyOld<RankTwoTensor>(_base_name + "stress")),
        _Jacobian_mult_elastic(getMaterialPropertyByName<RankFourTensor>(_base_name + "Jacobian_mult")),
        _component(getParam<unsigned int>("component")),
        _ndisp(coupledComponents("displacements")),
        _disp_var(_ndisp),
        _conc_coupled(isCoupled("concentration")),
        _conc_var(_conc_coupled ? coupled("concentration") : 0),
        _deigenstrain_dC(_conc_coupled ? &getMaterialPropertyDerivative<RankTwoTensor>(
                                         getParam<std::string>("concentration_eigenstrain_name"),
                                         getVar("concentration", 0)->name())
                                   : nullptr)

{
      for (unsigned int i = 0; i < _ndisp; ++i)
        _disp_var[i] = coupled("displacements", i);
}

Real 
Stresschemicalpotential::computeQpResidual()
{
    return _u[_qp] + _stress[_qp].doubleContraction((*_deigenstrain_dC)[_qp])*_test[_i][_qp];
    
    return 0.0;
}

Real
Stresschemicalpotential::computeQpJacobian()
{
    return _phi[_j][_qp] * _test[_i][_qp];
}

// Todo implement off diagonal terms