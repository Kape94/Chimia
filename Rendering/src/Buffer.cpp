#include "Buffer.h"

#include "BufferUtils.h"
#include "OpenGLDefs.h"

USING_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

Buffer::Buffer(Buffer&& other) noexcept
  : m_VAO(other.m_VAO)
  , m_VBO(other.m_VBO)
  , m_nVertices(other.m_nVertices)
{
  other.m_VAO = 0;
  other.m_VBO = 0;
  other.m_nVertices = 0;
}

//---------------------------------------------------------------------------------------

Buffer&
Buffer::operator=(Buffer&& other) noexcept
{
  if (&other != this) {
    m_VAO = other.m_VAO;
    m_VBO = other.m_VBO;
    m_nVertices = other.m_nVertices;

    other.m_VAO = 0;
    other.m_VBO = 0;
    other.m_nVertices = 0;
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

Buffer::Buffer(const float* vertexData,
               const unsigned nVertexDataItems,
               const ShaderAttributes& shaderAttributes)
{
  Create(vertexData, nVertexDataItems, shaderAttributes);
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
  const unsigned vertexDataSize = static_cast<unsigned>(vertexData.size());
  Create(vertexData.data(), vertexDataSize, shaderAttributes);
}

//---------------------------------------------------------------------------------------

void
Buffer::Create(const float* vertexData,
               const unsigned nVertexDataItems,
               const ShaderAttributes& shaderAttributes)
{
  Clear();

  LoadDataInGPU(vertexData, nVertexDataItems);
  BufferUtils::LinkShaderAttributes(shaderAttributes);

  const unsigned itemsPerVertex =
    BufferUtils::ComputeTotalEntriesOfAttributes(shaderAttributes);
  m_nVertices = nVertexDataItems / itemsPerVertex;
}

//---------------------------------------------------------------------------------------

void
Buffer::LoadDataInGPU(const float* vertexData, const unsigned nVertexDataItems)
{
  glGenVertexArrays(1, &m_VAO);
  glBindVertexArray(m_VAO);

  LoadVertexDataInGPU(vertexData, nVertexDataItems);
}

//---------------------------------------------------------------------------------------

void
Buffer::LoadVertexDataInGPU(const float* vertexData,
                            const unsigned nVertexDataItems)
{
  const unsigned vertexDataSize = nVertexDataItems * sizeof(float);
  m_VBO = BufferUtils::CreateBufferAndLoadData(
    GL_ARRAY_BUFFER, vertexData, vertexDataSize);
}

//---------------------------------------------------------------------------------------

void
Buffer::Load(const float* vertexData, const unsigned nVertexDataItems)
{
  if (m_VAO == 0 || m_VBO == 0) {
    return;
  }

  const unsigned totalSize = nVertexDataItems * sizeof(float);
  BufferUtils::LoadDataOnBuffer(m_VBO, GL_ARRAY_BUFFER, vertexData, totalSize);
}

//---------------------------------------------------------------------------------------

unsigned
Buffer::GetVAO() const
{
  return m_VAO;
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
