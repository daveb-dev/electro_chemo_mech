/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ComputeHyperViscoIsoStress.C
 * Author: srinath
 * 
 * Created on March 14, 2019, 3:24 PM
 */

#include "ComputeHyperViscoIsoStress.h"

#include "StressUpdateBase.h"
#include "MooseException.h"

registerMooseObject("electro_chemo_mechApp", ComputeHyperViscoIsoStress);

template <>
InputParameters
validParams<ComputeHyperViscoIsoStress>() 
{
   InputParameters params = validParams<ComputeFiniteStrainElasticStress>();
  params.addClassDescription("Compute state (stress and internal parameters such as plastic "
                             "strains and internal parameters) using an iterative process.  "
                             "Combinations of creep models and plastic models may be used.");
  params.addParam<unsigned int>("max_iterations",
                                30,
                                "Maximum number of the stress update "
                                "iterations over the stress change after all "
                                "update materials are called");
  params.addParam<Real>("relative_tolerance",
                        1e-5,
                        "Relative convergence tolerance for the stress "
                        "update iterations over the stress change "
                        "after all update materials are called");
  params.addParam<Real>("absolute_tolerance",
                        1e-5,
                        "Absolute convergence tolerance for the stress "
                        "update iterations over the stress change "
                        "after all update materials are called");
  params.addParam<bool>(
      "internal_solve_full_iteration_history",
      false,
      "Set to true to output stress update iteration information over the stress change");
  params.addParam<bool>("perform_finite_strain_rotations",
                        true,
                        "Tensors are correctly rotated in "
                        "finite-strain simulations.  For "
                        "optimal performance you can set "
                        "this to 'false' if you are only "
                        "ever using small strains");
  MooseEnum tangent_operator("elastic nonlinear", "nonlinear");
  params.addParam<MooseEnum>(
      "tangent_operator",
      tangent_operator,
      "Type of tangent operator to return.  'elastic': return the "
      "elasticity tensor.  'nonlinear': return the full, general consistent tangent "
      "operator.");
  return params;
}

ComputeHyperViscoIsoStress::ComputeHyperViscoIsoStress(const InputParameters & parameters) 
    : ComputeFiniteStrainElasticStress(parameters),
     _max_iterations(parameters.get<unsigned int>("max_iterations")),
    _relative_tolerance(parameters.get<Real>("relative_tolerance")),
    _absolute_tolerance(parameters.get<Real>("absolute_tolerance")),
    _internal_solve_full_iteration_history(getParam<bool>("internal_solve_full_iteration_history")),
    _perform_finite_strain_rotations(getParam<bool>("perform_finite_strain_rotations")),
    _elastic_strain_old(getMaterialPropertyOld<RankTwoTensor>(_base_name + "elastic_strain")),
    _strain_increment(getMaterialProperty<RankTwoTensor>(_base_name + "strain_increment")),
    _inelastic_strain(declareProperty<RankTwoTensor>(_base_name + "combined_inelastic_strain")),
    _inelastic_strain_old(
        getMaterialPropertyOld<RankTwoTensor>(_base_name + "combined_inelastic_strain")),
    _tangent_operator_type(getParam<MooseEnum>("tangent_operator").getEnum<TangentOperatorEnum>()),
    _tangent_calculation_method(TangentCalculationMethod::ELASTIC),
    _matl_timestep_limit(declareProperty<Real>("matl_timestep_limit")),
    _identity_symmetric_four(RankFourTensor::initIdentitySymmetricFour), 
    _deformation_gradient(getMaterialProperty<RankTwoTensor>(_base_name + "deformation_gradient")),
    _deformation_gradient_old(getMaterialPropertyOld<RankTwoTensor>(_base_name + "deformation_gradient"))
{
}

void 
ComputeHyperViscoIsoStress::initQpStatefulProperties()
{
    ComputeStressBase::initQpStatefulProperties();
    _inelastic_strain[_qp].zero();
}

void
ComputeHyperViscoIsoStress::initialSetup()
{ 
    _is_elasticity_tensor_guaranteed_isotropic =
      hasGuaranteedMaterialProperty(_elasticity_tensor_name, Guarantee::ISOTROPIC);

}

void
ComputeHyperViscoIsoStress::computeQpStress()
{
  computeQpStressIntermediateConfiguration();
  if (_perform_finite_strain_rotations)
    finiteStrainRotation();

}