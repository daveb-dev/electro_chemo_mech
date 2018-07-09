//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#ifndef ELECTRO_CHEMO_MECHAPP_H
#define ELECTRO_CHEMO_MECHAPP_H

#include "MooseApp.h"

class electro_chemo_mechApp;

template <>
InputParameters validParams<electro_chemo_mechApp>();

class electro_chemo_mechApp : public MooseApp
{
public:
  electro_chemo_mechApp(InputParameters parameters);
  virtual ~electro_chemo_mechApp();

  static void registerApps();
  static void registerObjects(Factory & factory);
  static void registerObjectDepends(Factory & factory);
  static void associateSyntax(Syntax & syntax, ActionFactory & action_factory);
  static void associateSyntaxDepends(Syntax & syntax, ActionFactory & action_factory);
  static void registerExecFlags(Factory & factory);
};

#endif /* ELECTRO_CHEMO_MECHAPP_H */
