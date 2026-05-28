#include "Image.h"

// ----------------------------------------------------------------------------

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_MEDIA_NAMESPACE

// ----------------------------------------------------------------------------

namespace ImageUtils {
float
AsNormalizedFloat(const unsigned char component)
{
  return static_cast<float>(component) / 256.0f;
}

unsigned char
AsByte(const float component)
{
  return static_cast<unsigned char>(component * 256.0f);
}

}

// ----------------------------------------------------------------------------

Image::Image(const std::string& imagePath)
{
  int width = 0, height = 0, nChannels = 0;
  unsigned char* data =
    stbi_load(imagePath.c_str(), &width, &height, &nChannels, 0);

  assert(data != nullptr && "Media::Image: Couldn't open image");

  m_width = width;
  m_height = height;
  m_nChannels = nChannels;
  m_rawData = data;
}

// ----------------------------------------------------------------------------

Image::Image(const int width, const int height, const int nChannels)
  : Image(width, height, nChannels, [&]() {
    unsigned char* data = new unsigned char[width * height * nChannels];
    memset(data, 0, sizeof(unsigned char) * width * height * nChannels);
    return data;
  }())
{
}

// ----------------------------------------------------------------------------

Image::Image(const int width,
             const int height,
             const int nChannels,
             unsigned char* rawData)
  : m_width(width)
  , m_height(height)
  , m_nChannels(nChannels)
  , m_rawData(rawData)
{
}

// ----------------------------------------------------------------------------

Image::~Image()
{
  if (m_rawData != nullptr) {
    stbi_image_free(m_rawData);
    m_rawData = nullptr;
    m_width = 0;
    m_height = 0;
    m_nChannels = 0;
  }
}

// ----------------------------------------------------------------------------

glm::vec3
Image::GetRGBPixel(const int i, const int j) const
{
  assert(m_nChannels == 3 &&
         "Image::GetRGBPixel: image doesn't have 3 channels");

  const unsigned char* pixelData = GetPixelPointer(i, j);

  const unsigned char r = pixelData[0];
  const unsigned char g = pixelData[1];
  const unsigned char b = pixelData[2];

  return { ImageUtils::AsNormalizedFloat(r),
           ImageUtils::AsNormalizedFloat(g),
           ImageUtils::AsNormalizedFloat(b) };
}

// ----------------------------------------------------------------------------

glm::vec4
Image::GetRGBAPixel(const int i, const int j) const
{
  assert(m_nChannels == 4 &&
         "Image::GetRGBAPixel: image doesn't have 4 channels");

  const unsigned char* pixelData = GetPixelPointer(i, j);

  const unsigned char r = pixelData[0];
  const unsigned char g = pixelData[1];
  const unsigned char b = pixelData[2];
  const unsigned char a = pixelData[3];

  return { ImageUtils::AsNormalizedFloat(r),
           ImageUtils::AsNormalizedFloat(g),
           ImageUtils::AsNormalizedFloat(b),
           ImageUtils::AsNormalizedFloat(a) };
}

// ----------------------------------------------------------------------------

void
Image::SetRGBPixel(const glm::vec3& color, const int i, const int j)
{
  assert(m_nChannels == 3 && "SetRGBPixel: image doesn't have 3 channels");

  unsigned char* pixelData = GetPixelPointer(i, j);
  pixelData[0] = ImageUtils::AsByte(color.r);
  pixelData[1] = ImageUtils::AsByte(color.g);
  pixelData[2] = ImageUtils::AsByte(color.b);
}

// ----------------------------------------------------------------------------

void
Image::SetRGBAPixel(const glm::vec4& color, const int i, const int j)
{
  assert(m_nChannels == 4 && "SetRGBAPixel: image doesn't have 4 channels");

  unsigned char* pixelData = GetPixelPointer(i, j);
  pixelData[0] = ImageUtils::AsByte(color.r);
  pixelData[1] = ImageUtils::AsByte(color.g);
  pixelData[2] = ImageUtils::AsByte(color.b);
  pixelData[3] = ImageUtils::AsByte(color.a);
}

// ----------------------------------------------------------------------------

int
Image::Width() const
{
  return m_width;
}

// ----------------------------------------------------------------------------

int
Image::Height() const
{
  return m_height;
}

// ----------------------------------------------------------------------------

int
Image::NChannels() const
{
  return m_nChannels;
}

// ----------------------------------------------------------------------------

const unsigned char*
Image::RawData() const
{
  return m_rawData;
}

// ----------------------------------------------------------------------------

void
Image::Save(const std::string& path, const bool flipVertically)
{
  stbi_flip_vertically_on_write(flipVertically);
  stbi_write_bmp(path.c_str(), m_width, m_height, m_nChannels, m_rawData);
}

// ----------------------------------------------------------------------------

const unsigned char*
Image::GetPixelPointer(const int i, const int j) const
{
  const int pixelOffset = m_nChannels * (m_width * j + i);
  return m_rawData + pixelOffset;
}

// ----------------------------------------------------------------------------

unsigned char*
Image::GetPixelPointer(const int i, const int j)
{
  const Image* const_this = this;
  return const_cast<unsigned char*>(const_this->GetPixelPointer(i, j));
}

// ----------------------------------------------------------------------------