#pragma once

#include "AppToolkitDefs.h"

#include "imgui.h"

_BEGIN_APP_TOOLKIT_NAMESPACE

class IuiPresenter
{
public:
  virtual ~IuiPresenter() = default;

  virtual void Present() = 0;
};

_END_APP_TOOLKIT_NAMESPACE
