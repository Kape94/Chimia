#pragma once

#include "AppToolkitDefs.h"
#include "IuiPresenter.h"

#include <GLFW/glfw3.h>

_BEGIN_APP_TOOLKIT_NAMESPACE

class UiContext
{
public:
  void Init(GLFWwindow* window);

  void SetPresenter(IuiPresenter* _presenter);
  void PresentUI();

  void Cleanup();

private:
  IuiPresenter* uiPresenter = nullptr;
};

_END_APP_TOOLKIT_NAMESPACE
