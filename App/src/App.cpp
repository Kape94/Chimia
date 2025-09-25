#include "App.h"

#include "Application.h"

USING_CHIMIA_APP_NAMESPACE

//-----------------------------------------------------------------------------

IApp&
Chimia::App::GetApp()
{
  static AppImpl app;
  return app;
}

//-----------------------------------------------------------------------------
