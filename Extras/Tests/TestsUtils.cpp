#include "TestsUtils.h"

#include "Tester.h"

#include "Utils/ExtrasUtils.h"

#include <iostream>
#include <memory>

// ----------------------------------------------------------------------------

namespace TestsUtilsInternal {
std::unique_ptr<Tester> g_tester = nullptr;
const std::string GOLDEN_ARTIFACTS_DIR = "goldenArtifacts/";

namespace Dev {
bool g_shouldLogExpectedValues = false;
}
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
TestsUtils::ExpectValue(const float value,
                        const float expected,
                        const float error)
{
  if (TestsUtilsInternal::Dev::g_shouldLogExpectedValues) {
    std::cout << "[DEVELOPMENT] Logging expected value...\n";
    std::cout << "[DEVELOPMENT] Received: " << value << "\n";
    std::cout << "[DEVELOPMENT] Expected: " << expected << "\n";
    std::cout << "[DEVELOPMENT] Tolerance: " << error << std::endl;
    return;
  }

  const float diff = std::abs(value - expected);
  if (diff > error) {
    std::cout << "Value obtained is different from expected\n";
    std::cout << "Expected " << expected << " with tolerance " << error << "\n";
    std::cout << "Got " << value << std::endl;
    exit(1);
  }
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

void
TestsUtils::Development::ActivateValueLogging()
{
  TestsUtilsInternal::Dev::g_shouldLogExpectedValues = true;
}

// ----------------------------------------------------------------------------