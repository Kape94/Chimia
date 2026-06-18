#pragma once

#include "QuadsDrawingTest.h"

#include "Utils/Window.h"

void
TestImmediateModeTriangles(const ImmediateTrianglesTestInfo& testInfo,
                           Window& window);

void
TestRetainedModeTriangles(const ImmediateTrianglesTestInfo& testInfo,
                          Window& window);