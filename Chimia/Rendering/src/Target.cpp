#include "Target.h"
#include "FrameBuffer.h"
#include <memory>

// ----------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------

std::shared_ptr<Target>
Target::Create(const ShaderInstance& shader)
{
  return std::shared_ptr<Target>(new Target(shader));
}

// ----------------------------------------------------------------------------

std::shared_ptr<Target>
Target::Create(const ShaderInstance& shader,
               const FrameBufferInstance& framebuffer)
{
  return std::shared_ptr<Target>(new Target(shader, framebuffer));
}

// ----------------------------------------------------------------------------

Target::Target(const ShaderInstance& shader)
  : m_shader(shader)
  , m_framebuffer(FrameBuffer::DefaultFrameBuffer())
{
}

// ----------------------------------------------------------------------------

Target::Target(const ShaderInstance& shader,
               const FrameBufferInstance& framebuffer)
  : m_shader(shader)
  , m_framebuffer(framebuffer)
{
}

// ----------------------------------------------------------------------------