#pragma once

#include "MediaNamespaceDefs.h"

#include <string>

BEGIN_CHIMIA_MEDIA_NAMESPACE

class ImageFormat
{
public:
  enum Type
  {
    BMP = 1,
    PNG,
    UNKNOWN
  };

  static ImageFormat FromString(const std::string& extension);

  ImageFormat(const Type type);
  operator Type() const { return m_type; }

  std::string ToString() const;

private:
  ImageFormat() = delete;

  Type m_type = UNKNOWN;
};

END_CHIMIA_MEDIA_NAMESPACE