#include "TestsUtils.h"

#include "Tester.h"

#include "Utils/ExtrasUtils.h"

#include <memory>

// ----------------------------------------------------------------------------

namespace TestsUtilsInternal {
std::unique_ptr<Tester> g_tester = nullptr;
const std::string GOLDEN_ARTIFACTS_DIR = "goldenArtifacts/";
}

// ----------------------------------------------------------------------------

void
TestsUtils::InitTesting(char** argv, const Window& window)
{
  TestsUtilsInternal::g_tester.reset(
    new Tester(ExtrasUtils::GetCurrentAppDir(argv), window));
}

// ----------------------------------------------------------------------------

void
TestsUtils::ExpectImage(const std::string& imageFileName)
{
  using namespace TestsUtilsInternal;

  g_tester->TakeScreenshotAndAssert(GOLDEN_ARTIFACTS_DIR + imageFileName);
}

// ----------------------------------------------------------------------------