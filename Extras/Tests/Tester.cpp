#include "Tester.h"

#include "Utils/ImageUtils.h"
#include "Utils/SamplesUtils.h"

#include <iostream>

// ----------------------------------------------------------------------------

namespace TesterUtils {

void
ExitWithError()
{
  exit(1);
}

}

// ----------------------------------------------------------------------------

Tester::Tester(const std::string& workspaceDir, const Window& window)
  : m_workspaceDir(workspaceDir)
  , m_window(window)
{
  if (m_workspaceDir[m_workspaceDir.size() - 1] != '/') {
    m_workspaceDir += "/";
  }
}

// ----------------------------------------------------------------------------

void
Tester::TakeScreenshotAndAssert(const std::string& goldenImagePath) const
{
  const std::string tempOutputPath = m_workspaceDir + "output.bmp";
  SamplesUtils::SaveScreenshot(m_window, tempOutputPath);

  const Chimia::Media::Image output(tempOutputPath);
  const Chimia::Media::Image goldenImage(m_workspaceDir + goldenImagePath);

  const ImageUtils::ImageComparisonResult result =
    ImageUtils::Compare(output, goldenImage);

  AssertImageComparisonResult(result);
}

// ----------------------------------------------------------------------------

void
Tester::AssertImageComparisonResult(
  const ImageUtils::ImageComparisonResult& result) const
{
  const bool comparedWithoutErrors = result.errorMessage.empty();
  if (comparedWithoutErrors) {
    if (result.diff > m_imageErrorTolerance) {
      std::cout << "DIFF: " << result.diff << "\n\n";
      result.diffImage.SaveAsBmp(m_workspaceDir + "diff.bmp",
                                 false /*flipVertically*/);

      TesterUtils::ExitWithError();
    }
  } else {
    std::cout << "Couldn't compare the images:\n"
              << result.errorMessage << "\n";

    TesterUtils::ExitWithError();
  }
}

// ----------------------------------------------------------------------------