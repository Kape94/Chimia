#include "AppToolkit.h"

#include "Application.h"

_USING_APP_TOOLKIT_NAMESPACE

//-----------------------------------------------------------------------------

IApp&
AppToolkit::GetApp()
{
  static Application app;
  return app;
}

//-----------------------------------------------------------------------------
