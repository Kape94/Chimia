#include "InstancedRenderAction.h"

#include "IndexData.h"
#include "ShaderAttribute.h"
#include "VertexData.h"

//---------------------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

InstancedRenderAction::InstancedRenderAction(
  InstancedRenderAction&& other) noexcept
  : m_action(std::move(other.m_action))
{
}

//---------------------------------------------------------------------------------------

InstancedRenderAction&
InstancedRenderAction::operator=(InstancedRenderAction&& other) noexcept
{
  if (&other != this) {
    m_action = std::move(other.m_action);
  }

  return *this;
}

//---------------------------------------------------------------------------------------

InstancedRenderAction::~InstancedRenderAction()
{
  Clear();
}

//---------------------------------------------------------------------------------------

void
InstancedRenderAction::CreateInstanced(
  const VertexDataInstance& reusableVertexBuffer,
  const ShaderAttributes& vertexShaderAttributes,
  const InstancedDataInstance& instancesData,
  const ShaderAttributes& instanceShaderAttributes)
{
  m_action.CreateInstanced(reusableVertexBuffer,
                           vertexShaderAttributes,
                           instancesData,
                           instanceShaderAttributes);
}

//---------------------------------------------------------------------------------------

void
InstancedRenderAction::CreateInstanced(
  const VertexDataInstance& reusableVertexBuffer,
  const IndexDataInstance& reusableIndexBuffer,
  const ShaderAttributes& vertexShaderAttributes,
  const InstancedDataInstance& instancesData,
  const ShaderAttributes& instanceShaderAttributes)
{
  m_action.CreateInstanced(reusableVertexBuffer,
                           reusableIndexBuffer,
                           vertexShaderAttributes,
                           instancesData,
                           instanceShaderAttributes);
}

//---------------------------------------------------------------------------------------

void
InstancedRenderAction::RelinkInstancedData(
  const InstancedDataInstance& instancesData,
  const ShaderAttributes& instanceShaderAttributes)
{
  m_action.RelinkInstancedData(instancesData, instanceShaderAttributes);
}

//---------------------------------------------------------------------------------------

void
InstancedRenderAction::Clear()
{
  m_action.Clear();
}

//---------------------------------------------------------------------------------------

void
InstancedRenderAction::Render() const
{
  m_action.Render();
}

//---------------------------------------------------------------------------------------
