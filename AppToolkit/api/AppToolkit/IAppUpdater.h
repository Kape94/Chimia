#pragma once

#include "AppToolkitDefs.h"
#include "IAppUtils.h"

_BEGIN_APP_TOOLKIT_NAMESPACE

class IAppUpdater
{
public:
  virtual ~IAppUpdater() = default;

  virtual void Update(IAppUtils& appUtils) = 0;
};

_END_APP_TOOLKIT_NAMESPACE
