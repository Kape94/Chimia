#include "ImageFormat.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_MEDIA_NAMESPACE

// ----------------------------------------------------------------------------

ImageFormat
ImageFormat::FromString(const std::string& extension)
{
  for (Type t = BMP; t < UNKNOWN; t = static_cast<Type>(t + 1)) {
    const ImageFormat current(t);
    if (extension == current.ToString()) {
      return current;
    }
  }
  return ImageFormat(UNKNOWN);
}

// ----------------------------------------------------------------------------

ImageFormat::ImageFormat(const Type type)
  : m_type(type)
{
}

// ----------------------------------------------------------------------------

std::string
ImageFormat::ToString() const
{
  switch (m_type) {
    case BMP:
      return "bmp";
    case PNG:
      return "png";
    default:
      return "UNKNOWN";
  }
}

// ----------------------------------------------------------------------------
