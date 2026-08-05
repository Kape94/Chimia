#pragma once

//---------------------------------------------------------------------------------------

#include "Core/ClassDefs.h"
#include "RenderingNamespaceDefs.h"
#include "TextureUnit.h"

//---------------------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

class Texture2D
{
public:
  DEFAULT_CONSTUCTIBLE(Texture2D)
  NON_COPYABLE_NON_MOVABLE(Texture2D)

  Texture2D(const unsigned char* data,
            const unsigned width,
            const unsigned height);

  ~Texture2D();

  void Create(const unsigned char* data,
              const unsigned width,
              const unsigned height);

private:
  friend class BufferPrivate;
  void Use(const TextureUnit& textureUnit) const;

  void Clear();

  unsigned m_id = 0;
};

//---------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------