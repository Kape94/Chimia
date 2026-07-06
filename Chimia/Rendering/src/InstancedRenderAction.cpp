#include "InstancedRenderAction.h"

#include "BufferPrivate.h"
#include "BufferUtils.h"
#include "Core/Types.h"
#include "IndexedRenderAction.h"
#include "OpenGLDefs.h"
#include "ShaderAttribute.h"
#include <variant>

//---------------------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

InstancedRenderAction::InstancedRenderAction(
  InstancedRenderAction&& other) noexcept
  : m_baseBuffer(std::move(other.m_baseBuffer))
  , m_instancedVBO(other.m_instancedVBO)
  , m_nInstances(other.m_nInstances)
{
  other.m_instancedVBO = 0;
  other.m_nInstances = 0;
}

//---------------------------------------------------------------------------------------

InstancedRenderAction&
InstancedRenderAction::operator=(InstancedRenderAction&& other) noexcept
{
  if (&other != this) {
    m_instancedVBO = other.m_instancedVBO;
    m_nInstances = other.m_nInstances;
    m_baseBuffer = std::move(other.m_baseBuffer);

    other.m_instancedVBO = 0;
    other.m_nInstances = 0;
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
  const ReusableVertexBufferObject& reusableVertexBuffer,
  const ShaderAttributes& vertexShaderAttributes,
  const RawArrayView& instancesData,
  const ShaderAttributes& instanceShaderAttributes)
{
  m_baseBuffer = RenderAction();
  RenderAction& base = std::get<RenderAction>(m_baseBuffer);

  base.Create(reusableVertexBuffer, vertexShaderAttributes);

  CreateInstancedBuffer(instancesData, instanceShaderAttributes);
}

//---------------------------------------------------------------------------------------

void
InstancedRenderAction::CreateInstanced(
  const ReusableIndexedVertexBufferObject& reusableVertexBuffer,
  const ShaderAttributes& vertexShaderAttributes,
  const RawArrayView& instancesData,
  const ShaderAttributes& instanceShaderAttributes)
{
  m_baseBuffer = IndexedRenderAction();
  IndexedRenderAction& base = std::get<IndexedRenderAction>(m_baseBuffer);

  base.Create(reusableVertexBuffer, vertexShaderAttributes);

  CreateInstancedBuffer(instancesData, instanceShaderAttributes);
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
  m_baseBuffer = IndexedRenderAction();
  IndexedRenderAction& base = std::get<IndexedRenderAction>(m_baseBuffer);

  base.Create(vertexData, indexData, shaderAttributes);

  CreateInstancedBuffer(instancesData, instanceShaderAttributes);
}

//---------------------------------------------------------------------------------------

void
InstancedRenderAction::CreateInstanced(
  const RawDataView& vertexData,
  const ShaderAttributes& shaderAttributes,
  const RawArrayView& instancesData,
  const ShaderAttributes& instanceShaderAttributes)
{
  m_baseBuffer = RenderAction();
  RenderAction& base = std::get<RenderAction>(m_baseBuffer);

  base.Create(vertexData, shaderAttributes);

  CreateInstancedBuffer(instancesData, instanceShaderAttributes);
}

//---------------------------------------------------------------------------------------

void
InstancedRenderAction::LoadInstancedData(const RawArrayView& instancesData)
{
  const unsigned totalSize = instancesData.TotalSize();
  BufferUtils::LoadDataOnBuffer(
    m_instancedVBO, GL_ARRAY_BUFFER, instancesData.array, totalSize);

  m_nInstances = instancesData.nItems;
}

//---------------------------------------------------------------------------------------

void
InstancedRenderAction::RecreateInstancedBuffer(
  const RawArrayView& instancesData,
  const ShaderAttributes& instanceShaderAttributes)
{
  /* A quick note: for some reason clearing the buffer and creating a new one
     right after doesn't works. While debugging I noticed that doing that way,
     the ID for the buffer wasn't getting changed, and this was kinda messing
     things in the driver. Doing this way (creating the new buffer first), the
     m_instancedVBO forcefully acquires a different ID, and I think for some
     reason this makes the process work in the graphics driver.
  */
  const unsigned VAO = GetBaseVAO();

  glBindVertexArray(VAO);
  unsigned oldVBO = m_instancedVBO;

  // A new buffer will be created and assigned to m_instancedVBO
  CreateInstancedBuffer(instancesData, instanceShaderAttributes);

  ClearInstancesBuffer(oldVBO);
  glBindVertexArray(0);
}

//---------------------------------------------------------------------------------------

void
InstancedRenderAction::CreateInstancedBuffer(
  const RawArrayView& instancesData,
  const ShaderAttributes& instanceShaderAttributes)
{
  LoadInstancedDataInGPU(
    instancesData.array, instancesData.itemSize, instancesData.nItems);
  BufferUtils::LinkInstancedShaderAttributes(instanceShaderAttributes);

  m_nInstances = instancesData.nItems;
}

//---------------------------------------------------------------------------------------

void
InstancedRenderAction::LoadInstancedDataInGPU(const void* instancedData,
                                              const unsigned instanceDataSize,
                                              const unsigned nInstances)
{
  m_instancedVBO = BufferUtils::CreateBufferAndLoadData(
    GL_ARRAY_BUFFER, instancedData, instanceDataSize * nInstances);
}

//---------------------------------------------------------------------------------------

unsigned
InstancedRenderAction::GetBaseVAO()
{
  if (auto buffer = std::get_if<RenderAction>(&m_baseBuffer)) {
    return BufferPrivate::GetVAO(*buffer);
  } else if (auto indexedBuffer =
               std::get_if<IndexedRenderAction>(&m_baseBuffer)) {
    return BufferPrivate::GetVAO(*indexedBuffer);
  }
  return 0;
}

//---------------------------------------------------------------------------------------

void
InstancedRenderAction::Clear()
{
  ClearBaseBuffer();
  ClearInstancesBuffer(m_instancedVBO);
}

//---------------------------------------------------------------------------------------

void
InstancedRenderAction::ClearBaseBuffer()
{
  if (auto buffer = std::get_if<RenderAction>(&m_baseBuffer)) {
    buffer->Clear();
  } else if (auto indexedBuffer =
               std::get_if<IndexedRenderAction>(&m_baseBuffer)) {
    indexedBuffer->Clear();
  }
}

//---------------------------------------------------------------------------------------

void
InstancedRenderAction::ClearInstancesBuffer(unsigned& instanceVBO)
{
  if (instanceVBO != 0) {
    glDeleteBuffers(1, &instanceVBO);
    instanceVBO = 0;
  }
}

//---------------------------------------------------------------------------------------

void
InstancedRenderAction::Render() const
{
  if (auto indexedBuffer = std::get_if<IndexedRenderAction>(&m_baseBuffer)) {
    RenderWithIndexedBaseBuffer(*indexedBuffer);
  } else if (auto buffer = std::get_if<RenderAction>(&m_baseBuffer)) {
    RenderWithRegularBaseBuffer(*buffer);
  }
}

//---------------------------------------------------------------------------------------

void
InstancedRenderAction::RenderWithRegularBaseBuffer(
  const RenderAction& buffer) const
{
  const unsigned VAO = BufferPrivate::GetVAO(buffer);
  const unsigned nVertices = BufferPrivate::GetNVertices(buffer);

  glBindVertexArray(VAO);
  glDrawArraysInstanced(GL_TRIANGLES, 0, nVertices, m_nInstances);
}

//---------------------------------------------------------------------------------------

void
InstancedRenderAction::RenderWithIndexedBaseBuffer(
  const IndexedRenderAction& indexedBuffer) const
{
  const unsigned VAO = BufferPrivate::GetVAO(indexedBuffer);
  const unsigned nElements = BufferPrivate::GetNElements(indexedBuffer);

  glBindVertexArray(VAO);
  glDrawElementsInstanced(
    GL_TRIANGLES, nElements, GL_UNSIGNED_INT, 0, m_nInstances);
}

//---------------------------------------------------------------------------------------
