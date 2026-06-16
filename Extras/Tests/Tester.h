#pragma once

#include "Utils/ImageUtils.h"
#include "Utils/Window.h"

#include <string>

class Tester
{
public:
  Tester() = delete;
  Tester(const std::string& workspaceDir, const Window& window);

  void ExpectImage(const std::string& goldenImagePath) const;

  std::string GetWorkspaceDir() const;

  // Test development feature -------------------------------------------------
  enum class eImageLoggingMode
  {
    OFF,
    RGB,
    RGBA
  };

  void SetImageLoggingMode(const eImageLoggingMode loggingMode);
  // --------------------------------------------------------------------------

private:
  void TakeScreenshotAndAssert(const std::string& goldenImagePath) const;

  void TakeScreenshot(const std::string& outputPath, const int nChannels) const;

  void AssertImageComparisonResult(
    const ImageUtils::ImageComparisonResult& result) const;

  void ReportDiffAndExit(const ImageUtils::ImageComparisonResult& result) const;

  void ReportComparisonErrorAndExit(
    const ImageUtils::ImageComparisonResult& result) const;

  void LogImage(const std::string& imagePath) const;

  // Optionally configurable members
  float m_imageErrorTolerance = 0.0000001f;
  eImageLoggingMode m_imageLoggingMode = eImageLoggingMode::OFF;

  // Required members
  std::string m_workspaceDir;
  const Window& m_window;
};