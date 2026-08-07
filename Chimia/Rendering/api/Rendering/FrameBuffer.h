#pragma once

#include "Core/ClassDefs.h"
#include "RenderingNamespaceDefs.h"
#include "Texture2D.h"

#include <memory>

//---------------------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

class FrameBuffer
{
public:
  NON_COPYABLE_NON_MOVABLE(FrameBuffer)

  ~FrameBuffer();

  static std::shared_ptr<FrameBuffer> Create(const unsigned width,
                                             const unsigned height);

  const Texture2DInstance& GetTexture() const;

  static const std::shared_ptr<FrameBuffer>& DefaultFrameBuffer();

private:
  FrameBuffer() = default;

  void Clear();

  friend class BufferPrivate;
  void Use() const;

  unsigned int m_id = 0;

  Texture2DInstance m_frameTexture = nullptr;
  unsigned m_renderBufferId = 0;
};

using FrameBufferInstance = std::shared_ptr<FrameBuffer>;

//---------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------
