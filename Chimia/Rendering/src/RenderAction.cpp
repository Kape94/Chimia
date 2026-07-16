#include "RenderAction.h"

#include "BufferPrivate.h"
#include "Core/Types.h"
#include "ShaderAttribute.h"
#include "VertexBuffer.h"
#include "VertexRenderData.h"

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

RenderAction::RenderAction(const RawDataView& bufferData,
                           const ShaderAttributes& shaderAttributes)
{
  Create(bufferData, shaderAttributes);
}

//---------------------------------------------------------------------------------------

RenderAction::~RenderAction()
{
  Clear();
}

//---------------------------------------------------------------------------------------

void
RenderAction::Create(const VertexBuffer& reusableVertexBuffer,
                     const ShaderAttributes& shaderAttributes)
{
  const VertexRenderData& baseBuffer =
    BufferPrivate::GetBaseBuffer(reusableVertexBuffer);
  m_action.Create(baseBuffer, shaderAttributes);
}

//---------------------------------------------------------------------------------------

void
RenderAction::Create(const RawDataView& vertexData,
                     const ShaderAttributes& shaderAttributes)
{
  Clear();

  m_action.Create(vertexData, shaderAttributes);
}

//---------------------------------------------------------------------------------------

void
RenderAction::Load(const RawDataView& vertexData)
{
  m_action.LoadVertexData(vertexData);
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