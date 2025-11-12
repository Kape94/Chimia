#include "ReusableIndexedVertexBufferObject.h"

#include "BufferPrivate.h"
#include "BufferUtils.h"
#include "Core/Types.h"
#include "OpenGLDefs.h"
#include <utility>

// ----------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------

ReusableIndexedVertexBufferObject::ReusableIndexedVertexBufferObject(
  ReusableIndexedVertexBufferObject&& other)
  : m_vertexBufferObject(std::move(other.m_vertexBufferObject))
  , m_EBO(other.m_EBO)
  , m_nIndices(other.m_nIndices)
{
  other.m_EBO = 0;
  other.m_nIndices = 0;
}

// ----------------------------------------------------------------------------

ReusableIndexedVertexBufferObject&
ReusableIndexedVertexBufferObject::operator=(
  ReusableIndexedVertexBufferObject&& other)
{
  m_vertexBufferObject = std::move(other.m_vertexBufferObject);
  m_EBO = other.m_EBO;
  m_nIndices = other.m_nIndices;

  other.m_EBO = 0;
  other.m_nIndices = 0;

  return *this;
}

// ----------------------------------------------------------------------------

ReusableIndexedVertexBufferObject::~ReusableIndexedVertexBufferObject()
{
  Clear();
}

// ----------------------------------------------------------------------------

void
ReusableIndexedVertexBufferObject::Create(const RawDataView& vertexData,
                                          const unsigned nVertices,
                                          const RawArrayView& indexData)
{
  m_vertexBufferObject.Create(vertexData, nVertices);

  const unsigned indexDataSize = indexData.TotalSize();
  m_EBO = BufferUtils::CreateBufferAndLoadData(
    GL_ELEMENT_ARRAY_BUFFER, indexData.array, indexDataSize);

  m_nIndices = indexData.nItems;
}

// ----------------------------------------------------------------------------

void
ReusableIndexedVertexBufferObject::Clear()
{
  m_vertexBufferObject.Clear();
  if (m_EBO != 0) {
    glDeleteBuffers(1, &m_EBO);
    m_EBO = 0;
  }
  m_nIndices = 0;
}

// ----------------------------------------------------------------------------

void
ReusableIndexedVertexBufferObject::Bind() const
{
  BufferPrivate::Bind(m_vertexBufferObject);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
}

// ----------------------------------------------------------------------------

unsigned
ReusableIndexedVertexBufferObject::GetNIndices() const
{
  return m_nIndices;
}

// ----------------------------------------------------------------------------

const ReusableVertexBufferObject&
ReusableIndexedVertexBufferObject::GetBaseBuffer() const
{
  return m_vertexBufferObject;
}

// ----------------------------------------------------------------------------