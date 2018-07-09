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
  Moose::registerObjects(_factory);
  ModulesApp::registerObjects(_factory);
  electro_chemo_mechApp::registerObjectDepends(_factory);
  electro_chemo_mechApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  ModulesApp::associateSyntax(_syntax, _action_factory);
  electro_chemo_mechApp::associateSyntaxDepends(_syntax, _action_factory);
  electro_chemo_mechApp::associateSyntax(_syntax, _action_factory);

  Moose::registerExecFlags(_factory);
  ModulesApp::registerExecFlags(_factory);
  electro_chemo_mechApp::registerExecFlags(_factory);

  bool use_test_objs = getParam<bool>("allow_test_objects");
  if (use_test_objs)
  {
    electro_chemo_mechTestApp::registerObjects(_factory);
    electro_chemo_mechTestApp::associateSyntax(_syntax, _action_factory);
    electro_chemo_mechTestApp::registerExecFlags(_factory);
  }
}

electro_chemo_mechTestApp::~electro_chemo_mechTestApp() {}

void
electro_chemo_mechTestApp::registerApps()
{
  registerApp(electro_chemo_mechApp);
  registerApp(electro_chemo_mechTestApp);
}

void
electro_chemo_mechTestApp::registerObjects(Factory & /*factory*/)
{
  /* Uncomment Factory parameter and register your new test objects here! */
}

void
electro_chemo_mechTestApp::associateSyntax(Syntax & /*syntax*/, ActionFactory & /*action_factory*/)
{
  /* Uncomment Syntax and ActionFactory parameters and register your new test objects here! */
}

void
electro_chemo_mechTestApp::registerExecFlags(Factory & /*factory*/)
{
  /* Uncomment Factory parameter and register your new execute flags here! */
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
// External entry point for dynamic application loading
extern "C" void
electro_chemo_mechTestApp__registerApps()
{
  electro_chemo_mechTestApp::registerApps();
}

// External entry point for dynamic object registration
extern "C" void
electro_chemo_mechTestApp__registerObjects(Factory & factory)
{
  electro_chemo_mechTestApp::registerObjects(factory);
}

// External entry point for dynamic syntax association
extern "C" void
electro_chemo_mechTestApp__associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
  electro_chemo_mechTestApp::associateSyntax(syntax, action_factory);
}

// External entry point for dynamic execute flag loading
extern "C" void
electro_chemo_mechTestApp__registerExecFlags(Factory & factory)
{
  electro_chemo_mechTestApp::registerExecFlags(factory);
}
