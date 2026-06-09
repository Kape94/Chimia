#include "Tester.h"

#include "Media/ImageFormat.h"
#include "Utils/ImageUtils.h"
#include "Utils/SamplesUtils.h"

#include <ctime>
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
  const Chimia::Media::ImageFormat format =
    SamplesUtils::GetFileNameFormat(goldenImagePath);

  const std::string tempOutputPath =
    m_workspaceDir + "output." + format.ToString();

  if (format == Chimia::Media::ImageFormat::PNG) {
    SamplesUtils::SaveRGBAScreenshot(m_window, tempOutputPath);
  } else {
    SamplesUtils::SaveRGBScreenshot(m_window, tempOutputPath);
  }

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
      const bool hasAlpha = result.diffImage.NChannels() == 4;
      if (hasAlpha) {
        result.diffImage.SaveAsPng(m_workspaceDir + "diff.png",
                                   false /*flipVertically*/);
      } else {
        result.diffImage.SaveAsBmp(m_workspaceDir + "diff.bmp",
                                   false /*flipVertically*/);
      }

      TesterUtils::ExitWithError();
    }
  } else {
    std::cout << "Couldn't compare the images:\n"
              << result.errorMessage << "\n";

    TesterUtils::ExitWithError();
  }
}

// ----------------------------------------------------------------------------