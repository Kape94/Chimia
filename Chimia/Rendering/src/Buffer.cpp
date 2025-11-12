#include "Buffer.h"

#include "BufferPrivate.h"
#include "BufferUtils.h"
#include "Core/Types.h"
#include "OpenGLDefs.h"
#include "ShaderAttribute.h"

USING_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

Buffer::Buffer(Buffer&& other) noexcept
  : m_VAO(other.m_VAO)
  , m_VBO(other.m_VBO)
  , m_nVertices(other.m_nVertices)
  , m_sizePerVertex(other.m_sizePerVertex)
{
  other.m_VAO = 0;
  other.m_VBO = 0;
  other.m_nVertices = 0;
  other.m_sizePerVertex = 0;
}

//---------------------------------------------------------------------------------------

Buffer&
Buffer::operator=(Buffer&& other) noexcept
{
  if (&other != this) {
    m_VAO = other.m_VAO;
    m_VBO = other.m_VBO;
    m_nVertices = other.m_nVertices;
    m_sizePerVertex = other.m_sizePerVertex;

    other.m_VAO = 0;
    other.m_VBO = 0;
    other.m_nVertices = 0;
    other.m_sizePerVertex = 0;
  }

  return *this;
}

//---------------------------------------------------------------------------------------

Buffer::Buffer(const RawDataView& bufferData,
               const ShaderAttributes& shaderAttributes)
{
  Create(bufferData, shaderAttributes);
}

//---------------------------------------------------------------------------------------

Buffer::~Buffer()
{
  Clear();
}

//---------------------------------------------------------------------------------------

void
Buffer::Create(const ReusableVertexBufferObject& reusableVertexBuffer,
               const ShaderAttributes& shaderAttributes)
{
  Clear();

  glGenVertexArrays(1, &m_VAO);
  glBindVertexArray(m_VAO);

  BufferPrivate::Bind(reusableVertexBuffer);
  BufferUtils::LinkShaderAttributes(shaderAttributes);

  m_nVertices = BufferPrivate::GetNVertices(reusableVertexBuffer);

  // The size per vertex info is used for subdata operations, which are not
  // applicable when we use a reusable vertex buffer.
  m_sizePerVertex = 0;
}

//---------------------------------------------------------------------------------------

void
Buffer::Create(const RawDataView& vertexData,
               const ShaderAttributes& shaderAttributes)
{
  Clear();

  const size_t vertexDataSize = vertexData.size;
  LoadDataInGPU(vertexData.data, vertexDataSize);
  BufferUtils::LinkShaderAttributes(shaderAttributes);

  m_sizePerVertex = BufferUtils::ComputeTotalSizeOfAttributes(shaderAttributes);
  m_nVertices = vertexDataSize / m_sizePerVertex;
}

//---------------------------------------------------------------------------------------

void
Buffer::LoadDataInGPU(const void* vertexData, const unsigned vertexDataSize)
{
  glGenVertexArrays(1, &m_VAO);
  glBindVertexArray(m_VAO);

  LoadVertexDataInGPU(vertexData, vertexDataSize);
}

//---------------------------------------------------------------------------------------

void
Buffer::LoadVertexDataInGPU(const void* vertexData,
                            const unsigned vertexDataSize)
{
  m_VBO = BufferUtils::CreateBufferAndLoadData(
    GL_ARRAY_BUFFER, vertexData, vertexDataSize);
}

//---------------------------------------------------------------------------------------

void
Buffer::Load(const RawDataView& vertexData)
{
  if (m_VAO == 0 || m_VBO == 0) {
    return;
  }

  const size_t vertexDataSize = vertexData.size;
  BufferUtils::LoadDataOnBuffer(
    m_VBO, GL_ARRAY_BUFFER, vertexData.data, vertexDataSize);
  m_nVertices = vertexDataSize / m_sizePerVertex;
}

//---------------------------------------------------------------------------------------

unsigned
Buffer::GetVAO() const
{
  return m_VAO;
}

//---------------------------------------------------------------------------------------

unsigned
Buffer::GetNVertices() const
{
  return m_nVertices;
}

//---------------------------------------------------------------------------------------

void
Buffer::Clear()
{
  if (m_VAO != 0) {
    glDeleteVertexArrays(1, &m_VAO);
    m_VAO = 0;
  }
  if (m_VBO != 0) {
    glDeleteBuffers(1, &m_VBO);
    m_VBO = 0;
  }
}

//---------------------------------------------------------------------------------------

void
Buffer::Render() const
{
  glBindVertexArray(m_VAO);
  glDrawArrays(GL_TRIANGLES, 0, m_nVertices);
}

//---------------------------------------------------------------------------------------
