#pragma once

#include "AppToolkitDefs.h"

#include <GL/glew.h>

_BEGIN_APP_TOOLKIT_NAMESPACE

class IAppRenderer
{
public:
  virtual ~IAppRenderer() = default;

  virtual void Render() = 0;
};

_END_APP_TOOLKIT_NAMESPACE
