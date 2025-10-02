#pragma once

#include "AppDefs.h"
#include "InputDefs.h"

#include <GLFW/glfw3.h>

#include <map>

BEGIN_CHIMIA_APP_NAMESPACE

class InputStatus
{
public:
  bool IsPressed(const Key key);
  bool IsPressed(const MouseButton mouseButton);

  bool IsReleased(const Key key);
  bool IsReleased(const MouseButton mouseButton);

  bool IsHold(const Key key);
  bool IsHold(const MouseButton mouseButton);

  void Poll(GLFWwindow* window);

private:
  std::map<Key, bool> previousKeyState;
  std::map<Key, bool> currentKeyState;

  std::map<MouseButton, bool> previousMouseButtonState;
  std::map<MouseButton, bool> currentMouseButtonState;
};

END_CHIMIA_APP_NAMESPACE
