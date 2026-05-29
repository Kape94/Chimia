#include "ImageUtils.h"
#include "Media/Image.h"

#include <functional>

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

namespace {

float
ComponentsSum(const glm::vec3& v)
{
  return v.r + v.g + v.b;
}

// ----------------------------------------------------------------------------

float
ComponentsSum(const glm::vec4& v)
{
  return v.r + v.g + v.b + v.a;
}

// ----------------------------------------------------------------------------

glm::vec3
AbsDiff(const glm::vec3& v1, const glm::vec3& v2)
{
  return { std::abs(v1.r - v2.r),
           std::abs(v1.g - v2.g),
           std::abs(v1.b - v2.b) };
}

// ----------------------------------------------------------------------------

glm::vec4
AbsDiff(const glm::vec4& v1, const glm::vec4& v2)
{
  return { std::abs(v1.r - v2.r),
           std::abs(v1.g - v2.g),
           std::abs(v1.b - v2.b),
           std::abs(v1.a - v2.a) };
}

// ----------------------------------------------------------------------------

bool
ImagesDimensionsAreMatching(const Chimia::Media::Image& i1,
                            const Chimia::Media::Image& i2)
{
  return i1.Width() == i2.Width() && i1.Height() == i2.Height() &&
         i1.NChannels() == i2.NChannels();
}

// ----------------------------------------------------------------------------

void
VisitRGBImages(
  const Chimia::Media::Image& i1,
  const Chimia::Media::Image& i2,
  const std::function<void(const glm::vec3&, const glm::vec3&, int, int)>&
    handlePixels)
{
  for (int i = 0; i < i1.Width(); ++i) {
    for (int j = 0; j < i1.Height(); ++j) {
      const glm::vec3 c1 = i1.GetRGBPixel(i, j);
      const glm::vec3 c2 = i2.GetRGBPixel(i, j);

      handlePixels(c1, c2, i, j);
    }
  }
}

// ----------------------------------------------------------------------------

void
VisitRGBAImages(
  const Chimia::Media::Image& i1,
  const Chimia::Media::Image& i2,
  const std::function<void(const glm::vec4&, const glm::vec4&, int, int)>&
    handlePixels)
{
  for (int i = 0; i < i1.Width(); ++i) {
    for (int j = 0; j < i1.Height(); ++j) {
      const glm::vec4 c1 = i1.GetRGBAPixel(i, j);
      const glm::vec4 c2 = i2.GetRGBAPixel(i, j);

      handlePixels(c1, c2, i, j);
    }
  }
}

// ----------------------------------------------------------------------------

float
RelativePixelArea(const Chimia::Media::Image& image)
{
  const float totalPixelArea = (float)image.Width() * image.Height();
  return 1.0f / totalPixelArea;
}

// ----------------------------------------------------------------------------

ImageUtils::ImageComparisonResult
ComparisonError(const std::string& message)
{
  return { Chimia::Media::Image(0, 0, 0, nullptr), 100.0f, message };
}

// ----------------------------------------------------------------------------

ImageUtils::ImageComparisonResult
CompareRGBImages(const Chimia::Media::Image& i1, const Chimia::Media::Image& i2)
{
  Chimia::Media::Image diffImage(i1.Width(), i1.Height(), i1.NChannels());
  float diff = 0.0f;

  const float pixelContribution = RelativePixelArea(i1);
  VisitRGBImages(
    i1, i2, [&](const glm::vec3& p1, const glm::vec3& p2, int x, int y) {
      const glm::vec3 diffPixel = AbsDiff(p1, p2);

      diff += pixelContribution * ComponentsSum(diffPixel);
      diffImage.SetRGBPixel(diffPixel, x, y);
    });

  return { std::move(diffImage), diff, "" };
}

// ----------------------------------------------------------------------------

ImageUtils::ImageComparisonResult
CompareRGBAImages(const Chimia::Media::Image& i1,
                  const Chimia::Media::Image& i2)
{
  Chimia::Media::Image diffImage(i1.Width(), i1.Height(), i1.NChannels());
  float diff = 0.0f;

  const float pixelContribution = RelativePixelArea(i1);
  VisitRGBAImages(
    i1, i2, [&](const glm::vec4& p1, const glm::vec4& p2, int x, int y) {
      const glm::vec4 diffPixel = AbsDiff(p1, p2);

      diff += pixelContribution * ComponentsSum(diffPixel);
      diffImage.SetRGBAPixel(diffPixel, x, y);
    });

  return { std::move(diffImage), diff, "" };
}

}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

ImageUtils::ImageComparisonResult
ImageUtils::Compare(const Chimia::Media::Image& i1,
                    const Chimia::Media::Image& i2)
{
  if (!ImagesDimensionsAreMatching(i1, i2)) {
    return ComparisonError(
      "Images width, height or number of channles don't match");
  }

  if (i1.NChannels() == 3) {
    return CompareRGBImages(i1, i2);
  } else if (i1.NChannels() == 4) {
    return CompareRGBAImages(i1, i2);
  }

  return ComparisonError("Images are not in RGB or RGBA layout");
}

// ----------------------------------------------------------------------------