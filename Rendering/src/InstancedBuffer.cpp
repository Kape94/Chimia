#include "InstancedBuffer.h"

#include "BufferPrivate.h"
#include "BufferUtils.h"
#include "OpenGLDefs.h"

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
  const float* vertexData,
  const unsigned nVertexDataItems,
  const unsigned* indexData,
  const unsigned nIndexDataItems,
  const ShaderAttributes& shaderAttributes,
  const void* instancedData,
  const unsigned instanceDataSize,
  const unsigned nInstances,
  const ShaderAttributes& instanceShaderAttributes)
{
  m_baseBuffer.Create(
    vertexData, nVertexDataItems, indexData, nIndexDataItems, shaderAttributes);

  LoadInstancedDataInGPU(instancedData, instanceDataSize, nInstances);
  BufferUtils::LinkInstancedShaderAttributes(instanceShaderAttributes);

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
  m_baseBuffer.Clear();
  if (m_instancedVBO != 0) {
    glDeleteBuffers(1, &m_instancedVBO);
    m_instancedVBO = 0;
  }
}

//---------------------------------------------------------------------------------------

void
InstancedBuffer::Render() const
{
  const unsigned VAO = BufferPrivate::GetVAO(m_baseBuffer);
  const unsigned nElements = BufferPrivate::GetNElements(m_baseBuffer);

  glBindVertexArray(VAO);
  glDrawElementsInstanced(
    GL_TRIANGLES, nElements, GL_UNSIGNED_INT, 0, m_nInstances);
}

//---------------------------------------------------------------------------------------
