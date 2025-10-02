#pragma once

#include "AppDefs.h"
#include "IAppUtils.h"

BEGIN_CHIMIA_APP_NAMESPACE

class IAppUpdater
{
public:
  virtual ~IAppUpdater() = default;

  virtual void Update(IAppUtils& appUtils) = 0;
};

END_CHIMIA_APP_NAMESPACE
