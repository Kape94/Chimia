#include "Buffer.h"

#include "BufferUtils.h"
#include "OpenGLDefs.h"

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

Buffer::Buffer(const std::vector<float>& vertexData,
               const ShaderAttributes& shaderAttributes)
{
  Create(vertexData, shaderAttributes);
}

//---------------------------------------------------------------------------------------

Buffer::Buffer(const void* vertexData,
               const unsigned vertexDataSize,
               const ShaderAttributes& shaderAttributes)
{
  Create(vertexData, vertexDataSize, shaderAttributes);
}

//---------------------------------------------------------------------------------------

Buffer::~Buffer()
{
  Clear();
}

//---------------------------------------------------------------------------------------

void
Buffer::Create(const std::vector<float>& vertexData,
               const ShaderAttributes& shaderAttributes)
{
  const unsigned nFloatItems = static_cast<unsigned>(vertexData.size());
  const unsigned vertexDataSize = nFloatItems * sizeof(float);

  Create(vertexData.data(), vertexDataSize, shaderAttributes);
}

//---------------------------------------------------------------------------------------

void
Buffer::Create(const void* vertexData,
               const unsigned vertexDataSize,
               const ShaderAttributes& shaderAttributes)
{
  Clear();

  LoadDataInGPU(vertexData, vertexDataSize);
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
Buffer::Load(const void* vertexData, const unsigned vertexDataSize)
{
  if (m_VAO == 0 || m_VBO == 0) {
    return;
  }

  BufferUtils::LoadDataOnBuffer(
    m_VBO, GL_ARRAY_BUFFER, vertexData, vertexDataSize);
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
