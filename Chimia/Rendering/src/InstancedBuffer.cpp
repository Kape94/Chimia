#include "InstancedBuffer.h"

#include "BufferPrivate.h"
#include "BufferUtils.h"
#include "Core/Types.h"
#include "IndexedBuffer.h"
#include "OpenGLDefs.h"
#include "ShaderAttribute.h"
#include <variant>

//---------------------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

InstancedBuffer::InstancedBuffer(InstancedBuffer&& other) noexcept
  : m_baseBuffer(std::move(other.m_baseBuffer))
  , m_instancedVBO(other.m_instancedVBO)
  , m_nInstances(other.m_nInstances)
{
  other.m_instancedVBO = 0;
  other.m_nInstances = 0;
}

//---------------------------------------------------------------------------------------

InstancedBuffer&
InstancedBuffer::operator=(InstancedBuffer&& other) noexcept
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

InstancedBuffer::~InstancedBuffer()
{
  Clear();
}

//---------------------------------------------------------------------------------------

void
InstancedBuffer::CreateInstanced(
  const ReusableVertexBufferObject& reusableVertexBuffer,
  const ShaderAttributes& vertexShaderAttributes,
  const RawArrayView& instancesData,
  const ShaderAttributes& instanceShaderAttributes)
{
  m_baseBuffer = Buffer();
  Buffer& base = std::get<Buffer>(m_baseBuffer);

  base.Create(reusableVertexBuffer, vertexShaderAttributes);

  CreateInstancedBuffer(instancesData, instanceShaderAttributes);
}

//---------------------------------------------------------------------------------------

void
InstancedBuffer::CreateInstanced(
  const ReusableIndexedVertexBufferObject& reusableVertexBuffer,
  const ShaderAttributes& vertexShaderAttributes,
  const RawArrayView& instancesData,
  const ShaderAttributes& instanceShaderAttributes)
{
  m_baseBuffer = IndexedBuffer();
  IndexedBuffer& base = std::get<IndexedBuffer>(m_baseBuffer);

  base.Create(reusableVertexBuffer, vertexShaderAttributes);

  CreateInstancedBuffer(instancesData, instanceShaderAttributes);
}

//---------------------------------------------------------------------------------------

void
InstancedBuffer::CreateInstanced(
  const RawDataView& vertexData,
  const RawArrayView& indexData,
  const ShaderAttributes& shaderAttributes,
  const RawArrayView& instancesData,
  const ShaderAttributes& instanceShaderAttributes)
{
  m_baseBuffer = IndexedBuffer();
  IndexedBuffer& base = std::get<IndexedBuffer>(m_baseBuffer);

  base.Create(vertexData, indexData, shaderAttributes);

  CreateInstancedBuffer(instancesData, instanceShaderAttributes);
}

//---------------------------------------------------------------------------------------

void
InstancedBuffer::CreateInstanced(
  const RawDataView& vertexData,
  const ShaderAttributes& shaderAttributes,
  const RawArrayView& instancesData,
  const ShaderAttributes& instanceShaderAttributes)
{
  m_baseBuffer = Buffer();
  Buffer& base = std::get<Buffer>(m_baseBuffer);

  base.Create(vertexData, shaderAttributes);

  CreateInstancedBuffer(instancesData, instanceShaderAttributes);
}

//---------------------------------------------------------------------------------------

void
InstancedBuffer::LoadInstancedData(const RawArrayView& instancesData)
{
  const unsigned totalSize = instancesData.TotalSize();
  BufferUtils::LoadDataOnBuffer(
    m_instancedVBO, GL_ARRAY_BUFFER, instancesData.array, totalSize);

  m_nInstances = instancesData.nItems;
}

//---------------------------------------------------------------------------------------

void
InstancedBuffer::RecreateInstancedBuffer(
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
InstancedBuffer::CreateInstancedBuffer(
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
InstancedBuffer::LoadInstancedDataInGPU(const void* instancedData,
                                        const unsigned instanceDataSize,
                                        const unsigned nInstances)
{
  m_instancedVBO = BufferUtils::CreateBufferAndLoadData(
    GL_ARRAY_BUFFER, instancedData, instanceDataSize * nInstances);
}

//---------------------------------------------------------------------------------------

unsigned
InstancedBuffer::GetBaseVAO()
{
  if (auto buffer = std::get_if<Buffer>(&m_baseBuffer)) {
    return BufferPrivate::GetVAO(*buffer);
  } else if (auto indexedBuffer = std::get_if<IndexedBuffer>(&m_baseBuffer)) {
    return BufferPrivate::GetVAO(*indexedBuffer);
  }
  return 0;
}

//---------------------------------------------------------------------------------------

void
InstancedBuffer::Clear()
{
  ClearBaseBuffer();
  ClearInstancesBuffer(m_instancedVBO);
}

//---------------------------------------------------------------------------------------

void
InstancedBuffer::ClearBaseBuffer()
{
  if (auto buffer = std::get_if<Buffer>(&m_baseBuffer)) {
    buffer->Clear();
  } else if (auto indexedBuffer = std::get_if<IndexedBuffer>(&m_baseBuffer)) {
    indexedBuffer->Clear();
  }
}

//---------------------------------------------------------------------------------------

void
InstancedBuffer::ClearInstancesBuffer(unsigned& instanceVBO)
{
  if (instanceVBO != 0) {
    glDeleteBuffers(1, &instanceVBO);
    instanceVBO = 0;
  }
}

//---------------------------------------------------------------------------------------

void
InstancedBuffer::Render() const
{
  if (auto indexedBuffer = std::get_if<IndexedBuffer>(&m_baseBuffer)) {
    RenderWithIndexedBaseBuffer(*indexedBuffer);
  } else if (auto buffer = std::get_if<Buffer>(&m_baseBuffer)) {
    RenderWithRegularBaseBuffer(*buffer);
  }
}

//---------------------------------------------------------------------------------------

void
InstancedBuffer::RenderWithRegularBaseBuffer(const Buffer& buffer) const
{
  const unsigned VAO = BufferPrivate::GetVAO(buffer);
  const unsigned nVertices = BufferPrivate::GetNVertices(buffer);

  glBindVertexArray(VAO);
  glDrawArraysInstanced(GL_TRIANGLES, 0, nVertices, m_nInstances);
}

//---------------------------------------------------------------------------------------

void
InstancedBuffer::RenderWithIndexedBaseBuffer(
  const IndexedBuffer& indexedBuffer) const
{
  const unsigned VAO = BufferPrivate::GetVAO(indexedBuffer);
  const unsigned nElements = BufferPrivate::GetNElements(indexedBuffer);

  glBindVertexArray(VAO);
  glDrawElementsInstanced(
    GL_TRIANGLES, nElements, GL_UNSIGNED_INT, 0, m_nInstances);
}

//---------------------------------------------------------------------------------------
