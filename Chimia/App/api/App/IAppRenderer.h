#pragma once

#include "AppDefs.h"

#include <GL/glew.h>

BEGIN_CHIMIA_APP_NAMESPACE

class IAppRenderer
{
public:
  virtual ~IAppRenderer() = default;

  virtual void Render() = 0;
};

END_CHIMIA_APP_NAMESPACE
