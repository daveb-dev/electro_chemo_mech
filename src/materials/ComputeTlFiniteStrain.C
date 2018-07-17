//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ComputeTlFiniteStrain.h"
#include "Assembly.h"
#include "libmesh/quadrature.h"

registerMooseObject("electro_chemo_mechApp", ComputeTlFiniteStrain);

template <>
InputParameters
validParams<ComputeTlFiniteStrain>()
{
  InputParameters params = validParams<ComputeStrainBase>();
  params.addClassDescription("Compute a Total Lagrangian strain measure e.g. Lagrange Strain, Green-Lagrange, Logarithmic"); 
  // Currently using Lagrange Strain as default ... no other options given
  return params;
}

ComputeTlFiniteStrain::ComputeTlFiniteStrain(const InputParameters & parameters)
  : ComputeStrainBase(parameters),
        _deformation_gradient(declareProperty<RankTwoTensor>(_base_name + "deformation_gradient"))
{
}
void
ComputeTlFiniteStrain::initialSetup()
{
    ComputeStrainBase::initialSetup();
    _deformation_gradient[_qp].zero();
    _deformation_gradient[_qp].addIa(1.0);
} 

void
ComputeTlFiniteStrain::initQpStatefulProperties()
{
    _deformation_gradient[_qp].zero();
    _deformation_gradient[_qp].addIa(1.0);
}
 

void
ComputeTlFiniteStrain::computeProperties()
{
    Real average_deformation_gradient = 0.0;
    for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
    {
        //Deformation Gradient
        RankTwoTensor F ((*_grad_disp[0])[_qp],
                         (*_grad_disp[1])[_qp],
                         (*_grad_disp[2])[_qp]);
        //Deformation Gradient
        _deformation_gradient[_qp] = F;
        _deformation_gradient[_qp].addIa(1.0);
        if (_volumetric_locking_correction)
        {
            // Calculate average deformation gradient
            average_deformation_gradient += _deformation_gradient[_qp].det()*_JxW[_qp]*_coord[_qp];
        }
    }
    if (_volumetric_locking_correction)
    {
        average_deformation_gradient /= _current_elem_volume;
    }
    // Finalize Volumetric locking correction
    for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
    {
        if (_volumetric_locking_correction)
        {
            _deformation_gradient[_qp] *= std::cbrt(average_deformation_gradient/_deformation_gradient[_qp].det());
        }
        //Compute Left and Right Cauchy-Green Deformation Tensors (LCG --> C = FF^T and RCG --> B = F^TF)
        RankTwoTensor LCG(_deformation_gradient[_qp]); 
        LCG *=_deformation_gradient[_qp].transpose();
        RankTwoTensor II(RankTwoTensor::initIdentity);
        _total_strain[_qp] = LCG -II;
        if (_global_strain)
            _total_strain[_qp] += (*_global_strain)[_qp];
        _mechanical_strain[_qp] = _total_strain[_qp];
        // Remove Eigen strain
        for (auto es:_eigenstrains)
            _mechanical_strain[_qp] -= (*es)[_qp];
    }
    
}
