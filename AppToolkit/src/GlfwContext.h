#pragma once

#include "AppToolkitDefs.h"
#include "WindowProperties.h"

#include <string>

#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

_BEGIN_APP_TOOLKIT_NAMESPACE

class GlfwContext
{
public:
  void Init(const WindowProperties& windowProperties);

  bool ShouldClose() const;

  void PollEvents();

  void Swap();

  void Cleanup();

  GLFWwindow* GetWindow();

  glm::vec2 GetCursorPos() const;
  void SetCursorPos(const glm::vec2& pos);
  glm::vec2 GetMouseMotion() const;

private:
  GLFWwindow* window = nullptr;
};

_END_APP_TOOLKIT_NAMESPACE
