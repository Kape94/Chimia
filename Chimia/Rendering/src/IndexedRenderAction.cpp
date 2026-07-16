#include "IndexedRenderAction.h"

#include "BufferPrivate.h"
#include "Core/Types.h"
#include "GenericRenderAction.h"
#include "VertexRenderData.h"

//---------------------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

IndexedRenderAction::IndexedRenderAction(IndexedRenderAction&& other) noexcept
  : m_action(std::move(other.m_action))
{
}

//---------------------------------------------------------------------------------------

IndexedRenderAction&
IndexedRenderAction::operator=(IndexedRenderAction&& other) noexcept
{
  if (&other != this) {
    m_action = std::move(other.m_action);
  }

  return *this;
}

//---------------------------------------------------------------------------------------

IndexedRenderAction::IndexedRenderAction(
  const RawDataView& vertexData,
  const RawArrayView& indexData,
  const ShaderAttributes& shaderAttributes)
{
  Create(vertexData, indexData, shaderAttributes);
}

//---------------------------------------------------------------------------------------

IndexedRenderAction::~IndexedRenderAction()
{
  Clear();
}

//---------------------------------------------------------------------------------------

void
IndexedRenderAction::Create(const IndexedVertexBuffer& reusableVertexBuffer,
                            const ShaderAttributes& shaderAttributes)
{
  const VertexRenderData& baseBuffer =
    BufferPrivate::GetBaseBuffer(reusableVertexBuffer);
  m_action.Create(baseBuffer, shaderAttributes);
}

//---------------------------------------------------------------------------------------

void
IndexedRenderAction::Create(const RawDataView& vertexData,
                            const RawArrayView& indexData,
                            const ShaderAttributes& shaderAttributes)
{
  m_action.Create(vertexData, indexData, shaderAttributes);
}

//---------------------------------------------------------------------------------------

void
IndexedRenderAction::LoadVertexData(const RawDataView& vertexData)
{
  m_action.LoadVertexData(vertexData);
}

//---------------------------------------------------------------------------------------

void
IndexedRenderAction::LoadIndexData(const RawArrayView& indexData)
{
  m_action.LoadIndexData(indexData);
}

//---------------------------------------------------------------------------------------

void
IndexedRenderAction::Clear()
{
  m_action.Clear();
}

//---------------------------------------------------------------------------------------

void
IndexedRenderAction::Render() const
{
  m_action.Render();
}

//---------------------------------------------------------------------------------------
