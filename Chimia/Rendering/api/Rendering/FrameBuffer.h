#pragma once

#include "Core/ClassDefs.h"
#include "RenderingNamespaceDefs.h"
#include "Texture2D.h"

//---------------------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

class FrameBuffer
{
public:
  DEFAULT_CONSTUCTIBLE(FrameBuffer)
  NON_COPYABLE_NON_MOVABLE(FrameBuffer)

  FrameBuffer(const unsigned width, const unsigned height);

  ~FrameBuffer();

  void Create(const unsigned width, const unsigned height);

  void Use();

  void UseTexture(const TextureUnit& textureUnit);

  static void UseDefaultFrameBuffer();

private:
  void Clear();

  unsigned int m_id = 0;

  Texture2D m_frameTexture;
  unsigned m_renderBufferId = 0;
};

//---------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------
