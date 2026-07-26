#include "IndexedRenderAction.h"

#include "GenericRenderAction.h"
#include "IndexData.h"

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

IndexedRenderAction::~IndexedRenderAction()
{
  Clear();
}

//---------------------------------------------------------------------------------------

void
IndexedRenderAction::Create(const VertexDataInstance& reusableVertexBuffer,
                            const IndexDataInstance& reusableIndexBuffer,
                            const ShaderAttributes& shaderAttributes)
{
  m_action.Create(reusableVertexBuffer, reusableIndexBuffer, shaderAttributes);
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
