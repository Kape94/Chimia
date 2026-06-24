#include "CommonTestingFixture.h"
#include "Draw3D/Draw3D.h"

#include "Utils/Window.h"

#include "TestModelsDrawing.h"

#include "TestsUtils.h"

int
main(int argc, char** argv)
{
  constexpr unsigned SCR_WIDTH = 800;
  constexpr unsigned SCR_HEIGHT = 600;
  Window win(SCR_WIDTH, SCR_HEIGHT, "Test #6");

  TestsUtils::InitTesting(argv, win);

  Chimia::Draw3D::Initialize();

  CommonTestingConfig testInfo;
  testInfo.testName = "test6";
  testInfo.flushOnEvery = 1000;
  testInfo.shouldVerifyRetainedRemovals = true;
  TestRetainedModeModels(testInfo, win);

  return 0;
}
