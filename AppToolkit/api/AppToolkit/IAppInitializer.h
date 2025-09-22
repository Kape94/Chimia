#pragma once

#include "AppToolkitDefs.h"

_BEGIN_APP_TOOLKIT_NAMESPACE

class IAppInitializer
{
public:
  virtual ~IAppInitializer() = default;

  virtual void Initialize() = 0;
};

_END_APP_TOOLKIT_NAMESPACE
