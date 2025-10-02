#pragma once

#include "AppDefs.h"

BEGIN_CHIMIA_APP_NAMESPACE

class IAppCleanup
{
public:
  virtual ~IAppCleanup() = default;

  virtual void Cleanup() = 0;
};

END_CHIMIA_APP_NAMESPACE
