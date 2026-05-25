#pragma once

#include <utility>

class GLFWwindow;

//---------------------------------------------------------------------------------------

class Window
{
public:
  Window(const unsigned width, const unsigned height, const char* windowName);

  ~Window();

  bool ShouldClose() const;

  void Swap();

  void PollEvents();

  std::pair<int, int> GetFramebufferSize() const;

private:
  GLFWwindow* window = nullptr;
};

//---------------------------------------------------------------------------------------
