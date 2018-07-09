#include "electro_chemo_mechApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

template <>
InputParameters
validParams<electro_chemo_mechApp>()
{
  InputParameters params = validParams<MooseApp>();
  return params;
}

electro_chemo_mechApp::electro_chemo_mechApp(InputParameters parameters) : MooseApp(parameters)
{
  Moose::registerObjects(_factory);
  ModulesApp::registerObjects(_factory);
  electro_chemo_mechApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  ModulesApp::associateSyntax(_syntax, _action_factory);
  electro_chemo_mechApp::associateSyntax(_syntax, _action_factory);

  Moose::registerExecFlags(_factory);
  ModulesApp::registerExecFlags(_factory);
  electro_chemo_mechApp::registerExecFlags(_factory);
}

electro_chemo_mechApp::~electro_chemo_mechApp() {}

void
electro_chemo_mechApp::registerApps()
{
  registerApp(electro_chemo_mechApp);
}

void
electro_chemo_mechApp::registerObjects(Factory & factory)
{
    Registry::registerObjectsTo(factory, {"electro_chemo_mechApp"});
}

void
electro_chemo_mechApp::associateSyntax(Syntax & /*syntax*/, ActionFactory & action_factory)
{
  Registry::registerActionsTo(action_factory, {"electro_chemo_mechApp"});

  /* Uncomment Syntax parameter and register your new production objects here! */
}

void
electro_chemo_mechApp::registerObjectDepends(Factory & /*factory*/)
{
}

void
electro_chemo_mechApp::associateSyntaxDepends(Syntax & /*syntax*/, ActionFactory & /*action_factory*/)
{
}

void
electro_chemo_mechApp::registerExecFlags(Factory & /*factory*/)
{
  /* Uncomment Factory parameter and register your new execution flags here! */
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
extern "C" void
electro_chemo_mechApp__registerApps()
{
  electro_chemo_mechApp::registerApps();
}

extern "C" void
electro_chemo_mechApp__registerObjects(Factory & factory)
{
  electro_chemo_mechApp::registerObjects(factory);
}

extern "C" void
electro_chemo_mechApp__associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
  electro_chemo_mechApp::associateSyntax(syntax, action_factory);
}

extern "C" void
electro_chemo_mechApp__registerExecFlags(Factory & factory)
{
  electro_chemo_mechApp::registerExecFlags(factory);
}
