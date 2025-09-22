#include "Texture2D.h"

#include "OpenGLDefs.h"

//---------------------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

Texture2D::Texture2D(const unsigned char* data,
                     const unsigned width,
                     const unsigned height)
{
  Create(data, width, height);
}

//---------------------------------------------------------------------------------------

Texture2D::~Texture2D()
{
  Clear();
}

//---------------------------------------------------------------------------------------

void
Texture2D::Create(const unsigned char* data,
                  const unsigned width,
                  const unsigned height)
{
  Clear();

  glGenTextures(1, &id);

  glBindTexture(GL_TEXTURE_2D, id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(
    GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(
    GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
}

//---------------------------------------------------------------------------------------

void
Texture2D::Use(const TextureUnit& textureUnit)
{
  const unsigned textureUnitID = static_cast<unsigned>(textureUnit);
  glActiveTexture(GL_TEXTURE0 + textureUnitID);
  glBindTexture(GL_TEXTURE_2D, id);
}

//---------------------------------------------------------------------------------------

unsigned
Texture2D::GetId() const
{
  return id;
}

//---------------------------------------------------------------------------------------

void
Texture2D::Clear()
{
  if (id != 0) {
    glDeleteTextures(1, &id);
    id = 0;
  }
}

//---------------------------------------------------------------------------------------
