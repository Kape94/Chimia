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