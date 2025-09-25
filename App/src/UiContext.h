#pragma once

#include "AppDefs.h"
#include "IuiPresenter.h"

#include <GLFW/glfw3.h>

BEGIN_CHIMIA_APP_NAMESPACE

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

END_CHIMIA_APP_NAMESPACE
