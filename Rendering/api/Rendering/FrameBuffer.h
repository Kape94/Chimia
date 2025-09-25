#pragma once

#include "RenderingNamespaceDefs.h"
#include "Texture2D.h"

//---------------------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

class FrameBuffer
{
public:
  FrameBuffer() = default;

  FrameBuffer(const unsigned width, const unsigned height);

  ~FrameBuffer();

  void Create(const unsigned width, const unsigned height);

  void Use();

  void UseTexture(const TextureUnit& textureUnit);

  void Clear();

  static void UseDefaultFrameBuffer();

private:
  unsigned int id = 0;

  Texture2D frameTexture;
  unsigned renderBufferId = 0;
};

//---------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------
