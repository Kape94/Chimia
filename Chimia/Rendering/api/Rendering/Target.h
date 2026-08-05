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

  static std::shared_ptr<Target> Create(const Shader& shader);
  static std::shared_ptr<Target> Create(const Shader& shader,
                                        const FrameBuffer& framebuffer);

private:
  Target(const Shader& shader);
  Target(const Shader& shader, const FrameBuffer& framebuffer);

  friend class BufferPrivate;

  const Shader& m_shader;
  const FrameBuffer& m_framebuffer;
};

using TargetInstance = std::shared_ptr<Target>;

//---------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------
