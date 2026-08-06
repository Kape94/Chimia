#pragma once

//---------------------------------------------------------------------------------------

#include "Core/ClassDefs.h"
#include "RenderingNamespaceDefs.h"
#include "TextureUnit.h"

#include <memory>

//---------------------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

class Texture2D
{
public:
  NON_COPYABLE_NON_MOVABLE(Texture2D)

  ~Texture2D();

  static std::shared_ptr<Texture2D> Create(const unsigned char* data,
                                           const unsigned width,
                                           const unsigned height);

private:
  Texture2D() = default;

  friend class BufferPrivate;
  void Use(const TextureUnit& textureUnit) const;

  void Clear();

  unsigned m_id = 0;
};

using Texture2DInstance = std::shared_ptr<Texture2D>;

//---------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------