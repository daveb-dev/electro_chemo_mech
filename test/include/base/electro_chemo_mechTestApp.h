//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#ifndef ELECTRO_CHEMO_MECHTESTAPP_H
#define ELECTRO_CHEMO_MECHTESTAPP_H

#include "MooseApp.h"

class electro_chemo_mechTestApp;

template <>
InputParameters validParams<electro_chemo_mechTestApp>();

class electro_chemo_mechTestApp : public MooseApp
{
public:
  electro_chemo_mechTestApp(InputParameters parameters);
  virtual ~electro_chemo_mechTestApp();

  static void registerApps();
  static void registerAll(Factory & f, ActionFactory & af, Syntax & s, bool use_test_objs = false);
};

#endif /* ELECTRO_CHEMO_MECHTESTAPP_H */
