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
    electro_chemo_mechApp::registerAll(_factory, _action_factory, _syntax);
}

electro_chemo_mechApp::~electro_chemo_mechApp() {}

void
electro_chemo_mechApp::registerAll(Factory &f, ActionFactory & af, Syntax & s)
{
    ModulesApp::registerAll(f, af, s);
    Registry::registerObjectsTo(f, {"electro_chemo_mechApp"});
    Registry::registerActionsTo(af, {"electro_chemo_mechApp"});

  /* register custom execute flags, action syntax, etc. here */
}

void
electro_chemo_mechApp::registerApps()
{
  registerApp(electro_chemo_mechApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
extern "C" void
electro_chemo_mechApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  electro_chemo_mechApp::registerAll(f, af, s);
}
extern "C" void
electro_chemo_mechApp__registerApps()
{
  electro_chemo_mechApp::registerApps();
}
