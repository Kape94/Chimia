#pragma once

//---------------------------------------------------------------------------------------

#include "Core/ClassDefs.h"
#include "FrameBuffer.h"
#include "RenderingNamespaceDefs.h"
#include "Shader.h"

#include <memory>

//---------------------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

class Target
{
public:
  NON_DEFAULT_CONSTRUCTIBLE(Target)
  DEFAULT_DESTRUCTIBLE(Target)
  NON_COPYABLE_NON_MOVABLE(Target)

  static std::shared_ptr<Target> Create(const ShaderInstance& shader);
  static std::shared_ptr<Target> Create(const ShaderInstance& shader,
                                        const FrameBufferInstance& framebuffer);

private:
  Target(const ShaderInstance& shader);
  Target(const ShaderInstance& shader, const FrameBufferInstance& framebuffer);

  friend class BufferPrivate;

  ShaderInstance m_shader;
  FrameBufferInstance m_framebuffer;
};

using TargetInstance = std::shared_ptr<Target>;

//---------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------
