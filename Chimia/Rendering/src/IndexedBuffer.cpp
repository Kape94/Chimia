#include "IndexedBuffer.h"

#include "BufferPrivate.h"
#include "BufferUtils.h"
#include "Core/Types.h"
#include "OpenGLDefs.h"

//---------------------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

IndexedRenderAction::IndexedRenderAction(IndexedRenderAction&& other) noexcept
  : m_EBO(other.m_EBO)
  , m_nElements(other.m_nElements)
  , m_baseBuffer(std::move(other.m_baseBuffer))
{
  other.m_EBO = 0;
  other.m_nElements = 0;
}

//---------------------------------------------------------------------------------------

IndexedRenderAction&
IndexedRenderAction::operator=(IndexedRenderAction&& other) noexcept
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

IndexedRenderAction::IndexedRenderAction(
  const RawDataView& vertexData,
  const RawArrayView& indexData,
  const ShaderAttributes& shaderAttributes)
{
  Create(vertexData, indexData, shaderAttributes);
}

//---------------------------------------------------------------------------------------

IndexedRenderAction::~IndexedRenderAction()
{
  Clear();
}

//---------------------------------------------------------------------------------------

void
IndexedRenderAction::Create(
  const ReusableIndexedVertexBufferObject& reusableVertexBuffer,
  const ShaderAttributes& shaderAttributes)
{
  Clear();

  m_baseBuffer.Create(BufferPrivate::GetBaseVertexBuffer(reusableVertexBuffer),
                      shaderAttributes);
  BufferPrivate::Bind(reusableVertexBuffer);

  m_nElements = BufferPrivate::GetNElements(reusableVertexBuffer);
}

//---------------------------------------------------------------------------------------

void
IndexedRenderAction::Create(const RawDataView& vertexData,
                            const RawArrayView& indexData,
                            const ShaderAttributes& shaderAttributes)
{
  Clear();

  m_baseBuffer.Create(vertexData, shaderAttributes);

  const unsigned nIndexDataItems = indexData.nItems;
  LoadIndexDataInGPU(reinterpret_cast<const unsigned*>(indexData.array),
                     nIndexDataItems);
  m_nElements = nIndexDataItems;
}

//---------------------------------------------------------------------------------------

void
IndexedRenderAction::LoadIndexDataInGPU(const unsigned* indexData,
                                        const unsigned nIndexDataItems)
{
  const unsigned indexDataSize = nIndexDataItems * sizeof(unsigned);
  m_EBO = BufferUtils::CreateBufferAndLoadData(
    GL_ELEMENT_ARRAY_BUFFER, indexData, indexDataSize);
}

//---------------------------------------------------------------------------------------

void
IndexedRenderAction::LoadVertexData(const RawDataView& vertexData)
{
  m_baseBuffer.Load(vertexData);
}

//---------------------------------------------------------------------------------------

void
IndexedRenderAction::LoadIndexData(const RawArrayView& indexData)
{
  if (GetVAO() == 0 || m_EBO == 0) {
    return;
  }

  const unsigned nIndexValues = indexData.nItems;
  BufferUtils::LoadDataOnBuffer(
    m_EBO, GL_ELEMENT_ARRAY_BUFFER, indexData.array, indexData.TotalSize());

  m_nElements = nIndexValues;
}

//---------------------------------------------------------------------------------------

unsigned
IndexedRenderAction::GetVAO() const
{
  return BufferPrivate::GetVAO(m_baseBuffer);
}

//---------------------------------------------------------------------------------------

unsigned
IndexedRenderAction::GetNElements() const
{
  return m_nElements;
}

//---------------------------------------------------------------------------------------

void
IndexedRenderAction::Clear()
{
  m_baseBuffer.Clear();
  if (m_EBO != 0) {
    glDeleteBuffers(1, &m_EBO);
    m_EBO = 0;
  }
}

//---------------------------------------------------------------------------------------

void
IndexedRenderAction::Render() const
{
  glBindVertexArray(BufferPrivate::GetVAO(m_baseBuffer));
  glDrawElements(GL_TRIANGLES, m_nElements, GL_UNSIGNED_INT, 0);
}

//---------------------------------------------------------------------------------------
