#pragma once

//---------------------------------------------------------------------------------------

#include "RenderingNamespaceDefs.h"
#include "TextureUnit.h"

//---------------------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

class Texture2D
{
public:
  Texture2D() = default;

  Texture2D(const unsigned char* data,
            const unsigned width,
            const unsigned height);

  ~Texture2D();

  Texture2D(const Texture2D& other) = delete;
  Texture2D& operator=(const Texture2D& other) = delete;

  Texture2D(Texture2D&& other) = delete;
  Texture2D& operator=(Texture2D&& other) = delete;

  void Create(const unsigned char* data,
              const unsigned width,
              const unsigned height);

  void Use(const Rendering::TextureUnit& textureUnit);

  unsigned GetId() const;

  void Clear();

private:
  unsigned id = 0;
};

//---------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------