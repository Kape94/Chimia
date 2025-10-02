#include "IndexedBuffer.h"

#include "BufferPrivate.h"
#include "BufferUtils.h"
#include "OpenGLDefs.h"

//---------------------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

IndexedBuffer::IndexedBuffer(IndexedBuffer&& other) noexcept
  : m_EBO(other.m_EBO)
  , m_nElements(other.m_nElements)
  , m_baseBuffer(std::move(other.m_baseBuffer))
{
  other.m_EBO = 0;
  other.m_nElements = 0;
}

//---------------------------------------------------------------------------------------

IndexedBuffer&
IndexedBuffer::operator=(IndexedBuffer&& other) noexcept
{
  if (&other != this) {
    m_EBO = other.m_EBO;
    m_nElements = other.m_nElements;
    m_baseBuffer = std::move(other.m_baseBuffer);

    other.m_EBO = 0;
    other.m_nElements = 0;
  }

  return *this;
}

//---------------------------------------------------------------------------------------

IndexedBuffer::IndexedBuffer(const std::vector<float>& vertexData,
                             const std::vector<unsigned>& indexData,
                             const ShaderAttributes& shaderAttributes)
{
  Create(vertexData, indexData, shaderAttributes);
}

//---------------------------------------------------------------------------------------

IndexedBuffer::IndexedBuffer(const float* vertexData,
                             const unsigned nVertexDataItems,
                             const unsigned* indexData,
                             const unsigned nIndexDataItems,
                             const ShaderAttributes& shaderAttributes)
{
  Create(
    vertexData, nVertexDataItems, indexData, nIndexDataItems, shaderAttributes);
}

//---------------------------------------------------------------------------------------

IndexedBuffer::~IndexedBuffer()
{
  Clear();
}

//---------------------------------------------------------------------------------------

void
IndexedBuffer::Create(const std::vector<float>& vertexData,
                      const std::vector<unsigned>& indexData,
                      const ShaderAttributes& shaderAttributes)
{
  const unsigned vertexDataSize = static_cast<unsigned>(vertexData.size());
  const unsigned indexDataSize = static_cast<unsigned>(indexData.size());
  Create(vertexData.data(),
         vertexDataSize,
         indexData.data(),
         indexDataSize,
         shaderAttributes);
}

//---------------------------------------------------------------------------------------

void
IndexedBuffer::Create(const float* vertexData,
                      const unsigned nVertexDataItems,
                      const unsigned* indexData,
                      const unsigned nIndexDataItems,
                      const ShaderAttributes& shaderAttributes)
{
  Clear();

  m_baseBuffer.Create(vertexData, nVertexDataItems, shaderAttributes);

  LoadIndexDataInGPU(indexData, nIndexDataItems);
  m_nElements = nIndexDataItems;
}

//---------------------------------------------------------------------------------------

void
IndexedBuffer::LoadIndexDataInGPU(const unsigned* indexData,
                                  const unsigned nIndexDataItems)
{
  const unsigned indexDataSize = nIndexDataItems * sizeof(unsigned);
  m_EBO = BufferUtils::CreateBufferAndLoadData(
    GL_ELEMENT_ARRAY_BUFFER, indexData, indexDataSize);
}

//---------------------------------------------------------------------------------------

unsigned
IndexedBuffer::GetVAO() const
{
  return BufferPrivate::GetVAO(m_baseBuffer);
}

//---------------------------------------------------------------------------------------

unsigned
IndexedBuffer::GetNElements() const
{
  return m_nElements;
}

//---------------------------------------------------------------------------------------

void
IndexedBuffer::Clear()
{
  m_baseBuffer.Clear();
  if (m_EBO != 0) {
    glDeleteBuffers(1, &m_EBO);
    m_EBO = 0;
  }
}

//---------------------------------------------------------------------------------------

void
IndexedBuffer::Render() const
{
  glBindVertexArray(BufferPrivate::GetVAO(m_baseBuffer));
  glDrawElements(GL_TRIANGLES, m_nElements, GL_UNSIGNED_INT, 0);
}

//---------------------------------------------------------------------------------------
