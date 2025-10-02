#pragma once

#include "AppDefs.h"

BEGIN_CHIMIA_APP_NAMESPACE

class IAppInitializer
{
public:
  virtual ~IAppInitializer() = default;

  virtual void Initialize() = 0;
};

END_CHIMIA_APP_NAMESPACE
