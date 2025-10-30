#include "ReusableVertexBufferObject.h"

#include "BufferUtils.h"
#include "OpenGLDefs.h"

// ----------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------

ReusableVertexBufferObject::ReusableVertexBufferObject(
  ReusableVertexBufferObject&& other)
  : m_VBO(other.m_VBO)
  , m_sizePerVertex(other.m_sizePerVertex)
  , m_nVertices(other.m_nVertices)
{
  other.m_VBO = 0;
  other.m_sizePerVertex = 0;
  other.m_nVertices = 0;
}

// ----------------------------------------------------------------------------

ReusableVertexBufferObject&
ReusableVertexBufferObject::operator=(ReusableVertexBufferObject&& other)
{
  m_VBO = other.m_VBO;
  m_sizePerVertex = other.m_sizePerVertex;
  m_nVertices = other.m_nVertices;

  other.m_VBO = 0;
  other.m_sizePerVertex = 0;
  other.m_nVertices = 0;

  return *this;
}

// ----------------------------------------------------------------------------

ReusableVertexBufferObject::~ReusableVertexBufferObject()
{
  Clear();
}

// ----------------------------------------------------------------------------

void
ReusableVertexBufferObject::Create(const void* vertexData,
                                   const unsigned vertexDataSize,
                                   const unsigned nVertices)
{
  m_VBO = BufferUtils::CreateBufferAndLoadData(
    GL_ARRAY_BUFFER, vertexData, vertexDataSize);

  m_nVertices = nVertices;
  m_sizePerVertex = vertexDataSize / nVertices;
}

// ----------------------------------------------------------------------------

void
ReusableVertexBufferObject::Clear()
{
  if (m_VBO != 0) {
    glDeleteBuffers(1, &m_VBO);
    m_VBO = 0;
  }
  m_sizePerVertex = 0;
  m_nVertices = 0;
}

// ----------------------------------------------------------------------------

void
ReusableVertexBufferObject::Bind() const
{
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
}

// ----------------------------------------------------------------------------

unsigned
ReusableVertexBufferObject::GetNVertices() const
{
  return m_nVertices;
}

// ----------------------------------------------------------------------------
