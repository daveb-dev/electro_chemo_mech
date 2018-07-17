/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Compute2DTlFiniteStrain.h
 * Author: srinath
 *
 * Created on July 17, 2018, 10:35 AM
 */

#ifndef COMPUTE2DTLFINITESTRAIN_H
#define COMPUTE2DTLFINITESTRAIN_H

#include "ComputeTlFiniteStrain.h"

class Compute2DTlFiniteStrain : public ComputeTlFiniteStrain

{
public:
    Compute2DTlFiniteStrain(const InputParameters &parameters);
protected:
    void initialSetup() override;
    virtual void displacementIntegrityCheck() override;
    virtual void computeProperties() override;
    
    //virtual Real computeOutOfPlaneGradDisp() = 0;
//    const bool _out_of_plane_strain_coupled;
//    const VariableValue & _out_of_plane_strain;
    
    const unsigned int _out_of_plane_direction;
    
private:

};

#endif /* COMPUTE2DTLFINITESTRAIN_H */

