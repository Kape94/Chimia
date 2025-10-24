#include "InstancedBuffer.h"

#include "BufferPrivate.h"
#include "BufferUtils.h"
#include "IndexedBuffer.h"
#include "OpenGLDefs.h"
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
  const void* instancedData,
  const unsigned instancedDataSize,
  const unsigned nInstances,
  const ShaderAttributes& instanceShaderAttributes)
{
  m_baseBuffer = IndexedBuffer();
  IndexedBuffer& base = std::get<IndexedBuffer>(m_baseBuffer);

  base.Create(reusableVertexBuffer);

  LoadInstancedDataInGPU(instancedData, instancedDataSize, nInstances);
  BufferUtils::LinkInstancedShaderAttributes(instanceShaderAttributes);

  m_nInstances = nInstances;
}

//---------------------------------------------------------------------------------------

void
InstancedBuffer::CreateInstanced(
  const void* vertexData,
  const unsigned vertexDataSize,
  const unsigned* indexData,
  const unsigned nIndexDataItems,
  const ShaderAttributes& shaderAttributes,
  const void* instancedData,
  const unsigned instancedDataSize,
  const unsigned nInstances,
  const ShaderAttributes& instanceShaderAttributes)
{
  m_baseBuffer = IndexedBuffer();
  IndexedBuffer& base = std::get<IndexedBuffer>(m_baseBuffer);

  base.Create(
    vertexData, vertexDataSize, indexData, nIndexDataItems, shaderAttributes);

  LoadInstancedDataInGPU(instancedData, instancedDataSize, nInstances);
  BufferUtils::LinkInstancedShaderAttributes(instanceShaderAttributes);

  m_nInstances = nInstances;
}

//---------------------------------------------------------------------------------------

void
InstancedBuffer::CreateInstanced(
  const void* vertexData,
  const unsigned vertexDataSize,
  const ShaderAttributes& shaderAttributes,
  const void* instancedData,
  const unsigned instancedDataSize,
  const unsigned nInstances,
  const ShaderAttributes& instanceShaderAttributes)
{
  m_baseBuffer = Buffer();
  Buffer& base = std::get<Buffer>(m_baseBuffer);

  base.Create(vertexData, vertexDataSize, shaderAttributes);

  LoadInstancedDataInGPU(instancedData, instancedDataSize, nInstances);
  BufferUtils::LinkInstancedShaderAttributes(instanceShaderAttributes);

  m_nInstances = nInstances;
}

//---------------------------------------------------------------------------------------

void
InstancedBuffer::LoadInstancedData(const void* instancedData,
                                   const unsigned instancedDataSize,
                                   const unsigned nInstances)
{
  const unsigned totalSize = instancedDataSize * nInstances;
  BufferUtils::LoadDataOnBuffer(
    m_instancedVBO, GL_ARRAY_BUFFER, instancedData, totalSize);

  m_nInstances = nInstances;
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

void
InstancedBuffer::Clear()
{
  ClearBaseBuffer();
  if (m_instancedVBO != 0) {
    glDeleteBuffers(1, &m_instancedVBO);
    m_instancedVBO = 0;
  }
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
