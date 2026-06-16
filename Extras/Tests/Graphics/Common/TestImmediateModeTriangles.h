#pragma once

#include "Utils/Window.h"

#include <string>

struct ImmediateTrianglesTestInfo
{
  std::string testName = "";
  unsigned flushOnEvery = 1000;
};

void
TestImmediateModeTriangles(const ImmediateTrianglesTestInfo& testInfo,
                           Window& window);