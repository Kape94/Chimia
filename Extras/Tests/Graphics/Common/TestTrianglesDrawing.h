#pragma once

#include "QuadsDrawingFixture.h"

#include "Utils/Window.h"

void
TestImmediateModeTriangles(const TrianglesDrawingTestInfo& testInfo,
                           Window& window);

void
TestRetainedModeTriangles(const TrianglesDrawingTestInfo& testInfo,
                          Window& window);