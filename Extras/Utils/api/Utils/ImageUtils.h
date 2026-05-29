#pragma once

#include "Media/Image.h"

#include <string>

namespace ImageUtils {

struct ImageComparisonResult
{
  Chimia::Media::Image diffImage;
  float diff = 0.0f;
  std::string errorMessage;
};

ImageComparisonResult
Compare(const Chimia::Media::Image& i1, const Chimia::Media::Image& i2);

}