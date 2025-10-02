#pragma once

#include "AppDefs.h"
#include "WindowProperties.h"

#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

BEGIN_CHIMIA_APP_NAMESPACE

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

END_CHIMIA_APP_NAMESPACE
