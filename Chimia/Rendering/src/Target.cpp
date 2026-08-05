#include "Target.h"
#include "FrameBuffer.h"
#include <memory>

// ----------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------

std::shared_ptr<Target>
Target::Create(const Shader& shader)
{
  return std::shared_ptr<Target>(new Target(shader));
}

// ----------------------------------------------------------------------------

std::shared_ptr<Target>
Target::Create(const Shader& shader, const FrameBuffer& framebuffer)
{
  return std::shared_ptr<Target>(new Target(shader, framebuffer));
}

// ----------------------------------------------------------------------------

Target::Target(const Shader& shader)
  : m_shader(shader)
  , m_framebuffer(FrameBuffer::DefaultFrameBuffer())
{
}

// ----------------------------------------------------------------------------

Target::Target(const Shader& shader, const FrameBuffer& framebuffer)
  : m_shader(shader)
  , m_framebuffer(framebuffer)
{
}

// ----------------------------------------------------------------------------