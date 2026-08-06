#include "Texture2D.h"

#include "GLState.h"
#include "OpenGLDefs.h"

//---------------------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

Texture2D::~Texture2D()
{
  Clear();
}

//---------------------------------------------------------------------------------------

std::shared_ptr<Texture2D>
Texture2D::Create(const unsigned char* data,
                  const unsigned width,
                  const unsigned height)
{
  Texture2DInstance newTexture(new Texture2D);

  glGenTextures(1, &newTexture->m_id);

  GLState::BindTexture2D(newTexture->m_id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(
    GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(
    GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  return newTexture;
}

//---------------------------------------------------------------------------------------

void
Texture2D::Use(const TextureUnit& textureUnit) const
{
  const unsigned textureUnitID = static_cast<unsigned>(textureUnit);
  glActiveTexture(GL_TEXTURE0 + textureUnitID);
  GLState::BindTexture2D(m_id);
}

//---------------------------------------------------------------------------------------

void
Texture2D::Clear()
{
  if (m_id != 0) {
    glDeleteTextures(1, &m_id);
    m_id = 0;
  }
}

//---------------------------------------------------------------------------------------
