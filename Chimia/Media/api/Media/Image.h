#pragma once

#include "MediaNamespaceDefs.h"

#include "Core/ClassDefs.h"

#include <string>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

BEGIN_CHIMIA_MEDIA_NAMESPACE

class Image
{
public:
  NON_DEFAULT_CONSTRUCTIBLE(Image)
  NON_COPYABLE_NON_MOVABLE(Image)

  Image(const std::string& imagePath);
  Image(const int width, const int height, const int nChannels);
  Image(const int width,
        const int height,
        const int nChannels,
        unsigned char* rawData);

  ~Image();

  glm::vec3 GetRGBPixel(const int i, const int j) const;
  glm::vec4 GetRGBAPixel(const int i, const int j) const;

  void SetRGBPixel(const glm::vec3& color, const int i, const int j);
  void SetRGBAPixel(const glm::vec4& color, const int i, const int j);

  int Width() const;
  int Height() const;
  int NChannels() const;
  const unsigned char* RawData() const;

  void Save(const std::string& path, const bool flipVertically);

private:
  const unsigned char* GetPixelPointer(const int i, const int j) const;
  unsigned char* GetPixelPointer(const int i, const int j);

  unsigned char* m_rawData = nullptr;
  int m_width = 0;
  int m_height = 0;
  int m_nChannels = 0;
};

END_CHIMIA_MEDIA_NAMESPACE