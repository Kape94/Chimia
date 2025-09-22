#pragma once

#include "AppToolkitDefs.h"

_BEGIN_APP_TOOLKIT_NAMESPACE

class IAppCleanup
{
public:
  virtual ~IAppCleanup() = default;

  virtual void Cleanup() = 0;
};

_END_APP_TOOLKIT_NAMESPACE
