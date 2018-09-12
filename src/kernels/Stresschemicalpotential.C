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
    params.addRequiredCoupledVar("concentration",
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
        _elasticity_tensor(getMaterialPropertyByName<RankFourTensor>(_base_name + "elasticity_tensor")),
        _deformation_gradient(getMaterialPropertyByName<RankTwoTensor>(_base_name +  "deformation_gradient")),
        _chem_var(coupled("chemical_potential")),
        _component(getParam<unsigned int>("component")),
        _ndisp(coupledComponents("displacements")),
        _disp_var(_ndisp),
        _conc_coupled(isCoupled("concentration")),
        _conc_var(_conc_coupled ? coupled("concentration") : 0),
        _concentration(coupledValue("concentration")),
        _concentration_old(coupledValueOld("concentration")),
        _deigenstrain_dC(_conc_coupled ? &getMaterialPropertyDerivative<RankTwoTensor>(
                                         getParam<std::string>("concentration_eigenstrain_name"),
                                         getVar("concentration", 0)->name())
                                   : nullptr),
        _density(getMaterialProperty<Real>("density"))

{
      for (unsigned int i = 0; i < _ndisp; ++i)
        _disp_var[i] = coupled("displacements", i);
}

Real 
Stresschemicalpotential::computeQpResidual()
{
    Real J = _deformation_gradient[_qp].det();
    RankTwoTensor kirchoff_stress = _stress[_qp]*J;
    return (_u[_qp]*_density[_qp] + kirchoff_stress.trace()*(*_deigenstrain_dC)[_qp].trace()/3.0)
            *_test[_i][_qp];
    
//    return 0.0;
}

Real
Stresschemicalpotential::computeQpJacobian()
{
    return _density[_qp]*_test[_i][_qp]*_phi[_j][_qp] ;
}

Real
Stresschemicalpotential::computeQpOffDiagJacobian(unsigned int jvar)
{
    Real J = _deformation_gradient[_qp].det();
    const RankTwoTensor I(RankTwoTensor::initIdentity);   
    RankTwoTensor dstress_dc = ElasticityTensorTools::getIsotropicBulkModulus(_elasticity_tensor[_qp])*((*_deigenstrain_dC)[_qp]);
    Real tt = dstress_dc.trace() * (*_deigenstrain_dC)[_qp].trace();
    if (jvar == _conc_var)
    {
//        return -dstress_dc.trace() * _phi[_j][_qp] * _test[_i][_qp]/_density[_qp]/3.0;
//        Real deltac = (_concentration[_qp] - _concentration_old[_qp]);
//        if (fabs(deltac) > 1.0e-20 )
//        {
//            Real resid = ((_stress[_qp] - _stress_old[_qp]).trace())/deltac
//                    *((*_deigenstrain_dC)[_qp]).trace(); // /_density[_qp];
            Real resid = tt;
            resid *= _phi[_j][_qp] * _test[_i][_qp];
            return resid;
//        }
//        else {
//            return 0.0;
//        }
    } else
    {
        for (unsigned int coupled_component = 0; coupled_component < _ndisp; ++ coupled_component)
        {
            if (jvar == _disp_var[coupled_component])
            {
                return (  _elasticity_tensor[_qp] * ((*_deigenstrain_dC)[_qp]) 
                * _grad_phi[_j][_qp])(coupled_component) *_test[_i][_qp];

            }
        }
    }
//        return 0.0;
    return 0.0;
}
// Todo implement off diagonal terms