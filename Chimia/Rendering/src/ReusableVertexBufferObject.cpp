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
  , m_shaderAttributes(std::move(other.m_shaderAttributes))
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
  m_shaderAttributes = std::move(other.m_shaderAttributes);

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
                                   const ShaderAttributes& shaderAttributes)
{
  m_VBO = BufferUtils::CreateBufferAndLoadData(
    GL_ARRAY_BUFFER, vertexData, vertexDataSize);

  m_sizePerVertex = BufferUtils::ComputeTotalSizeOfAttributes(shaderAttributes);
  m_nVertices = vertexDataSize / m_sizePerVertex;

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
  m_sizePerVertex = 0;
  m_nVertices = 0;
  m_shaderAttributes.clear();
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

const ShaderAttributes&
ReusableVertexBufferObject::GetShaderAttributes() const
{
  return m_shaderAttributes;
}

// ----------------------------------------------------------------------------