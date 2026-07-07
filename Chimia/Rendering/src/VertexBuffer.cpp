#include "VertexBuffer.h"

#include "BufferUtils.h"
#include "Core/Types.h"
#include "OpenGLDefs.h"

// ----------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------

VertexBuffer::VertexBuffer(VertexBuffer&& other)
  : m_VBO(other.m_VBO)
  , m_sizePerVertex(other.m_sizePerVertex)
  , m_nVertices(other.m_nVertices)
  , m_currentSize(other.m_currentSize)
  , m_maximumSize(other.m_maximumSize)
{
  other.m_VBO = 0;
  other.m_sizePerVertex = 0;
  other.m_nVertices = 0;
  other.m_currentSize = 0;
  other.m_maximumSize = 0;
}

// ----------------------------------------------------------------------------

VertexBuffer&
VertexBuffer::operator=(VertexBuffer&& other)
{
  m_VBO = other.m_VBO;
  m_sizePerVertex = other.m_sizePerVertex;
  m_nVertices = other.m_nVertices;
  m_currentSize = other.m_currentSize;
  m_maximumSize = other.m_maximumSize;

  other.m_VBO = 0;
  other.m_sizePerVertex = 0;
  other.m_nVertices = 0;
  other.m_currentSize = 0;
  other.m_maximumSize = 0;

  return *this;
}

// ----------------------------------------------------------------------------

VertexBuffer::~VertexBuffer()
{
  Clear();
}

// ----------------------------------------------------------------------------

void
VertexBuffer::Create(const RawDataView& vertexData, const unsigned nVertices)
{
  const size_t vertexDataSize = vertexData.size;
  m_VBO = BufferUtils::CreateBufferAndLoadData(
    GL_ARRAY_BUFFER, vertexData.data, vertexDataSize);

  m_nVertices = nVertices;
  m_sizePerVertex = vertexDataSize / nVertices;

  m_currentSize = vertexDataSize;
  m_maximumSize = vertexDataSize;
}

// ----------------------------------------------------------------------------

void
VertexBuffer::Load(const RawDataView& data)
{
  const size_t vertexDataSize = data.size;

  BufferUtils::LoadDataOnBuffer(
    m_VBO, GL_ARRAY_BUFFER, data.data, vertexDataSize);
  m_nVertices = vertexDataSize / m_sizePerVertex;

  m_currentSize = vertexDataSize;
}

// ----------------------------------------------------------------------------

void
VertexBuffer::Clear()
{
  if (m_VBO != 0) {
    glDeleteBuffers(1, &m_VBO);
    m_VBO = 0;
  }

  m_sizePerVertex = 0;
  m_nVertices = 0;
  m_currentSize = 0;
  m_maximumSize = 0;
}

// ----------------------------------------------------------------------------

void
VertexBuffer::Bind() const
{
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
}

// ----------------------------------------------------------------------------

unsigned
VertexBuffer::GetNVertices() const
{
  return m_nVertices;
}

// ----------------------------------------------------------------------------
