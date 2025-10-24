#include "ReusableVertexBufferObject.h"

#include "BufferUtils.h"
#include "OpenGLDefs.h"

// ----------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------

ReusableVertexBufferObject::ReusableVertexBufferObject(
  ReusableVertexBufferObject&& other)
  : m_VBO(other.m_VBO)
  , m_EBO(other.m_EBO)
  , m_sizePerVertex(other.m_sizePerVertex)
  , m_nVertices(other.m_nVertices)
  , m_nIndices(other.m_nIndices)
  , m_shaderAttributes(std::move(other.m_shaderAttributes))
{
  other.m_VBO = 0;
  other.m_EBO = 0;
  other.m_sizePerVertex = 0;
  other.m_nVertices = 0;
  other.m_nIndices = 0;
}

// ----------------------------------------------------------------------------

ReusableVertexBufferObject&
ReusableVertexBufferObject::operator=(ReusableVertexBufferObject&& other)
{
  m_VBO = other.m_VBO;
  m_EBO = other.m_EBO;
  m_sizePerVertex = other.m_sizePerVertex;
  m_nVertices = other.m_nVertices;
  m_nIndices = other.m_nIndices;
  m_shaderAttributes = std::move(other.m_shaderAttributes);

  other.m_VBO = 0;
  other.m_EBO = 0;
  other.m_sizePerVertex = 0;
  other.m_nVertices = 0;
  other.m_nIndices = 0;

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
                                   const unsigned* indexData,
                                   const unsigned nIndexDataItems,
                                   const ShaderAttributes& shaderAttributes)
{
  m_VBO = BufferUtils::CreateBufferAndLoadData(
    GL_ARRAY_BUFFER, vertexData, vertexDataSize);

  const unsigned indexDataSize = nIndexDataItems * sizeof(unsigned);
  m_EBO = BufferUtils::CreateBufferAndLoadData(
    GL_ELEMENT_ARRAY_BUFFER, indexData, indexDataSize);

  m_sizePerVertex = BufferUtils::ComputeTotalSizeOfAttributes(shaderAttributes);
  m_nVertices = vertexDataSize / m_sizePerVertex;
  m_nIndices = nIndexDataItems;

  m_shaderAttributes = shaderAttributes;
}

// ----------------------------------------------------------------------------

void
ReusableVertexBufferObject::Clear()
{
  if (m_VBO != 0) {
    glDeleteBuffers(1, &m_VBO);
    m_VBO = 0;
  }
  if (m_EBO != 0) {
    glDeleteBuffers(1, &m_EBO);
    m_EBO = 0;
  }
}

// ----------------------------------------------------------------------------

void
ReusableVertexBufferObject::Bind() const
{
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
}

// ----------------------------------------------------------------------------

unsigned
ReusableVertexBufferObject::GetNVertices() const
{
  return m_nVertices;
}

// ----------------------------------------------------------------------------

unsigned
ReusableVertexBufferObject::GetNIndices() const
{
  return m_nIndices;
}

// ----------------------------------------------------------------------------

const ShaderAttributes&
ReusableVertexBufferObject::GetShaderAttributes() const
{
  return m_shaderAttributes;
}

// ----------------------------------------------------------------------------