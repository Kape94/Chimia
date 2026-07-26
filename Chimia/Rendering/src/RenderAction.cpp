#include "RenderAction.h"

#include "Core/Types.h"
#include "ShaderAttribute.h"

//---------------------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

RenderAction::RenderAction(RenderAction&& other) noexcept
  : m_action(std::move(other.m_action))
{
}

//---------------------------------------------------------------------------------------

RenderAction&
RenderAction::operator=(RenderAction&& other) noexcept
{
  if (&other != this) {
    m_action = std::move(other.m_action);
  }

  return *this;
}

//---------------------------------------------------------------------------------------

RenderAction::~RenderAction()
{
  Clear();
}

//---------------------------------------------------------------------------------------

void
RenderAction::Create(const VertexDataInstance& reusableVertexBuffer,
                     const ShaderAttributes& shaderAttributes)
{
  m_action.Create(reusableVertexBuffer, shaderAttributes);
}

//---------------------------------------------------------------------------------------

void
RenderAction::Clear()
{
  m_action.Clear();
}

//---------------------------------------------------------------------------------------

void
RenderAction::Render() const
{
  m_action.Render();
}

//---------------------------------------------------------------------------------------