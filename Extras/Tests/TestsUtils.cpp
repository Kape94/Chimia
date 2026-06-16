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

std::string
TestsUtils::GetTestingDirectory()
{
  return TestsUtilsInternal::g_tester->GetWorkspaceDir();
}

// ----------------------------------------------------------------------------

void
TestsUtils::ExpectImage(const std::string& imageFileName)
{
  using namespace TestsUtilsInternal;

  g_tester->ExpectImage(GOLDEN_ARTIFACTS_DIR + imageFileName);
}

// ----------------------------------------------------------------------------

void
TestsUtils::Development::ActivateRGBImageLogging()
{
  TestsUtilsInternal::g_tester->SetImageLoggingMode(
    Tester::eImageLoggingMode::RGB);
}

// ----------------------------------------------------------------------------

void
TestsUtils::Development::ActivateRGBAImageLogging()
{
  TestsUtilsInternal::g_tester->SetImageLoggingMode(
    Tester::eImageLoggingMode::RGBA);
}

// ----------------------------------------------------------------------------