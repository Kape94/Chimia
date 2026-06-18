#include "Draw3D/Draw3D.h"

#include "Utils/Window.h"

#include "TestImmediateModeTriangles.h"

#include "TestsUtils.h"

int
main(int argc, char** argv)
{
  constexpr unsigned SCR_WIDTH = 800;
  constexpr unsigned SCR_HEIGHT = 600;
  Window win(SCR_WIDTH, SCR_HEIGHT, "Test #4");

  TestsUtils::InitTesting(argv, win);

  Chimia::Draw3D::Initialize();

  ImmediateTrianglesTestInfo testInfo;
  testInfo.testName = "test4";
  testInfo.flushOnEvery = 1000;
  TestRetainedModeTriangles(testInfo, win);

  return 0;
}
