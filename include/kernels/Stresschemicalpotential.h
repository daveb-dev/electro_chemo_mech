/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Stresschemicalpotential.h
 * Author: srinath
 *
 * Created on August 22, 2018, 3:18 PM
 */

#ifndef STRESSCHEMICALPOTENTIAL_H
#define STRESSCHEMICALPOTENTIAL_H

#include "Kernel.h"
#include "DerivativeMaterialInterface.h"
#include "RankTwoTensor.h"
#include "RankFourTensor.h"

class Stresschemicalpotential;
class RankTwoTensor;
class RankFourTensor;

template<> 
InputParameters validParams<Stresschemicalpotential>();

class Stresschemicalpotential : public DerivativeMaterialInterface<Kernel>
{
public:
    Stresschemicalpotential(const InputParameters & parameters);
protected:
    virtual Real computeQpResidual();
    virtual Real computeQpJacobian();
//    virtual Real Real computeQpOffDiagJacobian(unsigned int) override;


    std::string _base_name;
    const VariableGradient & _grad_chemical_potential;

    const MaterialProperty<RankTwoTensor> & _stress;
    const MaterialProperty<RankTwoTensor> & _stress_old;
    const MaterialProperty<RankFourTensor> & _Jacobian_mult_elastic;
//    const MaterialProperty<RankTwoTensor> * _deformation_gradient;
//    const MaterialProperty<RankTwoTensor> * _deformation_gradient_old;


    const unsigned int _component;
    //Coupled Displacement variables
    unsigned int _ndisp;
    std::vector<unsigned int> _disp_var;

   const bool _conc_coupled;
   const unsigned int _conc_var;

    /// d(strain)/d(concentration), if computed by ComputeConcentrationEigenstrain
   const MaterialProperty<RankTwoTensor> * const _deigenstrain_dC;
    
private:

};

#endif /* STRESSCHEMICALPOTENTIAL_H */

