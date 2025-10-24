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

IndexedBuffer::IndexedBuffer(const void* vertexData,
                             const unsigned vertexDataSize,
                             const unsigned* indexData,
                             const unsigned nIndexDataItems,
                             const ShaderAttributes& shaderAttributes)
{
  Create(
    vertexData, vertexDataSize, indexData, nIndexDataItems, shaderAttributes);
}

//---------------------------------------------------------------------------------------

IndexedBuffer::~IndexedBuffer()
{
  Clear();
}

//---------------------------------------------------------------------------------------

void
IndexedBuffer::Create(const ReusableVertexBufferObject& reusableVertexBuffer)
{
  Clear();

  m_baseBuffer.Create(reusableVertexBuffer);

  m_nElements = BufferPrivate::GetNElements(reusableVertexBuffer);
}

//---------------------------------------------------------------------------------------

void
IndexedBuffer::Create(const std::vector<float>& vertexData,
                      const std::vector<unsigned>& indexData,
                      const ShaderAttributes& shaderAttributes)
{
  const unsigned vertexDataNFloats = static_cast<unsigned>(vertexData.size());
  const unsigned vertexDataSize = vertexDataNFloats * sizeof(float);

  const unsigned indexDataSize = static_cast<unsigned>(indexData.size());
  Create(vertexData.data(),
         vertexDataSize,
         indexData.data(),
         indexDataSize,
         shaderAttributes);
}

//---------------------------------------------------------------------------------------

void
IndexedBuffer::Create(const void* vertexData,
                      const unsigned vertexDataSize,
                      const unsigned* indexData,
                      const unsigned nIndexDataItems,
                      const ShaderAttributes& shaderAttributes)
{
  Clear();

  m_baseBuffer.Create(vertexData, vertexDataSize, shaderAttributes);

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

void
IndexedBuffer::LoadVertexData(const void* vertexData,
                              const unsigned vertexDataSize)
{
  m_baseBuffer.Load(vertexData, vertexDataSize);
}

//---------------------------------------------------------------------------------------

void
IndexedBuffer::LoadIndexData(const unsigned* indexData,
                             const unsigned nIndexValues)
{
  if (GetVAO() == 0 || m_EBO == 0) {
    return;
  }

  constexpr unsigned SIZE_PER_INDEX = sizeof(unsigned);
  const unsigned indexDataSize = nIndexValues * SIZE_PER_INDEX;

  BufferUtils::LoadDataOnBuffer(
    m_EBO, GL_ELEMENT_ARRAY_BUFFER, indexData, indexDataSize);

  m_nElements = nIndexValues;
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
