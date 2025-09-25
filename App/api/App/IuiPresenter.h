#pragma once

#include "AppDefs.h"

#include "imgui.h"

BEGIN_CHIMIA_APP_NAMESPACE

class IuiPresenter
{
public:
  virtual ~IuiPresenter() = default;

  virtual void Present() = 0;
};

END_CHIMIA_APP_NAMESPACE
