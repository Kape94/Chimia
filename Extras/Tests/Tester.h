#pragma once

#include "Utils/ImageUtils.h"
#include "Utils/Window.h"

#include <string>

class Tester
{
public:
  Tester() = delete;
  Tester(const std::string& workspaceDir, const Window& window);

  void TakeScreenshotAndAssert(const std::string& goldenImagePath) const;

  std::string GetWorkspaceDir() const;

private:
  void TakeScreenshot(const std::string& outputPath, const int nChannels) const;

  void AssertImageComparisonResult(
    const ImageUtils::ImageComparisonResult& result) const;

  void ReportDiffAndExit(const ImageUtils::ImageComparisonResult& result) const;

  void ReportComparisonErrorAndExit(
    const ImageUtils::ImageComparisonResult& result) const;

  // Optionally configurable members
  float m_imageErrorTolerance = 0.0000001f;

  // Required members
  std::string m_workspaceDir;
  const Window& m_window;
};