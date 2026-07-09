#include "GenericVertexBuffer.h"

#include "BufferUtils.h"
#include "Core/Types.h"
#include "OpenGLDefs.h"

// ----------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------

GenericVertexBuffer::GenericVertexBuffer(GenericVertexBuffer&& other)
  : m_VBO(other.m_VBO)
  , m_EBO(other.m_EBO)
  , m_sizePerVertex(other.m_sizePerVertex)
  , m_nVertices(other.m_nVertices)
  , m_nIndices(other.m_nIndices)
  , m_currentVertexSize(other.m_currentVertexSize)
  , m_maximumVertexSize(other.m_maximumVertexSize)
  , m_currentIndexSize(other.m_currentIndexSize)
  , m_maximumIndexSize(other.m_maximumIndexSize)
{
  other.m_VBO = 0;
  other.m_EBO = 0;
  other.m_sizePerVertex = 0;
  other.m_nVertices = 0;
  other.m_nIndices = 0;
  other.m_currentVertexSize = 0;
  other.m_maximumVertexSize = 0;
  other.m_currentIndexSize = 0;
  other.m_maximumIndexSize = 0;
}

// ----------------------------------------------------------------------------

GenericVertexBuffer&
GenericVertexBuffer::operator=(GenericVertexBuffer&& other)
{
  m_VBO = other.m_VBO;
  m_EBO = other.m_EBO;
  m_sizePerVertex = other.m_sizePerVertex;
  m_nVertices = other.m_nVertices;
  m_nIndices = other.m_nIndices;
  m_currentVertexSize = other.m_currentVertexSize;
  m_maximumVertexSize = other.m_maximumVertexSize;
  m_currentIndexSize = other.m_currentIndexSize;
  m_maximumIndexSize = other.m_maximumIndexSize;

  other.m_VBO = 0;
  other.m_EBO = 0;
  other.m_sizePerVertex = 0;
  other.m_nVertices = 0;
  other.m_nIndices = 0;
  other.m_currentVertexSize = 0;
  other.m_maximumVertexSize = 0;
  other.m_currentIndexSize = 0;
  other.m_maximumIndexSize = 0;

  return *this;
}

// ----------------------------------------------------------------------------

GenericVertexBuffer::~GenericVertexBuffer()
{
  Clear();
}

// ----------------------------------------------------------------------------

void
GenericVertexBuffer::Create(const RawDataView& vertexData,
                            const unsigned nVertices)
{
  Clear();

  AllocateVertexData(vertexData, nVertices);
}

// ----------------------------------------------------------------------------

void
GenericVertexBuffer::Create(const RawDataView& vertexData,
                            const unsigned nVertices,
                            const RawArrayView& indexData)
{
  Clear();

  AllocateVertexData(vertexData, nVertices);
  AllocateIndexData(indexData);
}

// ----------------------------------------------------------------------------

void
GenericVertexBuffer::AllocateVertexData(const RawDataView& vertexData,
                                        const unsigned nVertices)
{
  const size_t vertexDataSize = vertexData.size;
  m_VBO = BufferUtils::CreateBufferAndLoadData(
    GL_ARRAY_BUFFER, vertexData.data, vertexDataSize);

  m_nVertices = nVertices;
  m_sizePerVertex = vertexDataSize / nVertices;

  m_currentVertexSize = vertexDataSize;
  m_maximumVertexSize = vertexDataSize;
}

// ----------------------------------------------------------------------------

void
GenericVertexBuffer::AllocateIndexData(const RawArrayView& indexData)
{
  const unsigned indexDataSize = indexData.TotalSize();
  m_EBO = BufferUtils::CreateBufferAndLoadData(
    GL_ELEMENT_ARRAY_BUFFER, indexData.array, indexDataSize);

  m_nIndices = indexData.nItems;

  m_currentIndexSize = indexDataSize;
  m_maximumIndexSize = indexDataSize;
}

// ----------------------------------------------------------------------------

void
GenericVertexBuffer::LoadVertexData(const RawDataView& data)
{
  const size_t vertexDataSize = data.size;
  if (m_VBO == 0 || vertexDataSize > m_maximumVertexSize) {
    return;
  }

  BufferUtils::LoadDataOnBuffer(
    m_VBO, GL_ARRAY_BUFFER, data.data, vertexDataSize);
  m_nVertices = vertexDataSize / m_sizePerVertex;

  m_currentVertexSize = vertexDataSize;
}

// ----------------------------------------------------------------------------

void
GenericVertexBuffer::LoadIndexData(const RawArrayView& indexData)
{
  const size_t incomingSize = indexData.TotalSize();
  if (m_EBO == 0 || incomingSize > m_maximumIndexSize) {
    return;
  }

  const unsigned nIndexValues = indexData.nItems;
  BufferUtils::LoadDataOnBuffer(
    m_EBO, GL_ELEMENT_ARRAY_BUFFER, indexData.array, indexData.TotalSize());

  m_nIndices = nIndexValues;
  m_currentIndexSize = incomingSize;
}

// ----------------------------------------------------------------------------

void
GenericVertexBuffer::Clear()
{
  if (m_VBO != 0) {
    glDeleteBuffers(1, &m_VBO);
    m_VBO = 0;
  }
  if (m_EBO != 0) {
    glDeleteBuffers(1, &m_EBO);
    m_EBO = 0;
  }

  m_sizePerVertex = 0;
  m_nVertices = 0;
  m_nIndices = 0;
  m_currentVertexSize = 0;
  m_maximumVertexSize = 0;
  m_currentIndexSize = 0;
  m_maximumIndexSize = 0;
}

// ----------------------------------------------------------------------------

void
GenericVertexBuffer::Bind() const
{
  if (m_VBO != 0) {
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  }
  if (m_EBO != 0) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  }
}

// ----------------------------------------------------------------------------

unsigned
GenericVertexBuffer::GetNVertices() const
{
  return m_nVertices;
}

// ----------------------------------------------------------------------------

unsigned
GenericVertexBuffer::GetNIndices() const
{
  return m_nIndices;
}

// ----------------------------------------------------------------------------

bool
GenericVertexBuffer::HasIndices() const
{
  return m_EBO != 0;
}

// ----------------------------------------------------------------------------
