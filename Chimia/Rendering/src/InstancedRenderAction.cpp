#include "InstancedRenderAction.h"

#include "Core/Types.h"
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
  const RawArrayView& instancesData,
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
  const RawArrayView& instancesData,
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
InstancedRenderAction::CreateInstanced(
  const RawDataView& vertexData,
  const RawArrayView& indexData,
  const ShaderAttributes& shaderAttributes,
  const RawArrayView& instancesData,
  const ShaderAttributes& instanceShaderAttributes)
{
  m_action.CreateInstanced(vertexData,
                           indexData,
                           shaderAttributes,
                           instancesData,
                           instanceShaderAttributes);
}

//---------------------------------------------------------------------------------------

void
InstancedRenderAction::CreateInstanced(
  const RawDataView& vertexData,
  const ShaderAttributes& shaderAttributes,
  const RawArrayView& instancesData,
  const ShaderAttributes& instanceShaderAttributes)
{
  m_action.CreateInstanced(
    vertexData, shaderAttributes, instancesData, instanceShaderAttributes);
}

//---------------------------------------------------------------------------------------

void
InstancedRenderAction::LoadInstancedData(const RawArrayView& instancesData)
{
  m_action.LoadInstancedData(instancesData);
}

//---------------------------------------------------------------------------------------

void
InstancedRenderAction::RecreateInstancedBuffer(
  const RawArrayView& instancesData,
  const ShaderAttributes& instanceShaderAttributes)
{
  m_action.RecreateInstancedBuffer(instancesData, instanceShaderAttributes);
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
