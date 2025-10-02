#pragma once

#include "AppDefs.h"

#include <string>

BEGIN_CHIMIA_APP_NAMESPACE

struct WindowProperties
{
  unsigned width = 800;
  unsigned height = 600;
  std::string name = "New window";
  bool fullscreen = false;
};

END_CHIMIA_APP_NAMESPACE
