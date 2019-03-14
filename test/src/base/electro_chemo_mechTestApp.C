//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#include "electro_chemo_mechTestApp.h"
#include "electro_chemo_mechApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "MooseSyntax.h"
#include "ModulesApp.h"

//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#include "electro_chemo_mechTestApp.h"
#include "electro_chemo_mechApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "MooseSyntax.h"
#include "ModulesApp.h"

template <>
InputParameters
validParams<electro_chemo_mechTestApp>()
{
  InputParameters params = validParams<electro_chemo_mechApp>();
  return params;
}

electro_chemo_mechTestApp::electro_chemo_mechTestApp(InputParameters parameters) : MooseApp(parameters)
{
  electro_chemo_mechTestApp::registerAll(
      _factory, _action_factory, _syntax, getParam<bool>("allow_test_objects"));
}

electro_chemo_mechTestApp::~electro_chemo_mechTestApp() {}

void
electro_chemo_mechTestApp::registerAll(Factory & f, ActionFactory & af, Syntax & s, bool use_test_objs)
{
  electro_chemo_mechApp::registerAll(f, af, s);
  if (use_test_objs)
  {
    Registry::registerObjectsTo(f, {"electro_chemo_mechTestApp"});
    Registry::registerActionsTo(af, {"electro_chemo_mechTestApp"});
  }
}

void
electro_chemo_mechTestApp::registerApps()
{
  registerApp(electro_chemo_mechApp);
  registerApp(electro_chemo_mechTestApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
// External entry point for dynamic application loading
extern "C" void
electro_chemo_mechTestApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  electro_chemo_mechTestApp::registerAll(f, af, s);
}
extern "C" void
electro_chemo_mechTestApp__registerApps()
{
  electro_chemo_mechTestApp::registerApps();
}
