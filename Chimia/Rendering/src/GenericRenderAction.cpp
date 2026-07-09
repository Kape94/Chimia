#include "GenericRenderAction.h"

#include "BufferPrivate.h"
#include "BufferUtils.h"
#include "Core/Types.h"
#include "GenericVertexBuffer.h"
#include "OpenGLDefs.h"
#include "ShaderAttribute.h"

//---------------------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

GenericRenderAction::GenericRenderAction(GenericRenderAction&& other) noexcept
  : m_VAO(other.m_VAO)
  , m_ownBuffer(std::move(other.m_ownBuffer))
  , m_referenceBuffer(other.m_referenceBuffer)
{
  other.m_VAO = 0;
  other.m_referenceBuffer = nullptr;
}

//---------------------------------------------------------------------------------------

GenericRenderAction&
GenericRenderAction::operator=(GenericRenderAction&& other) noexcept
{
  if (&other != this) {
    m_VAO = other.m_VAO;
    m_ownBuffer = std::move(other.m_ownBuffer);
    m_referenceBuffer = other.m_referenceBuffer;

    other.m_VAO = 0;
    other.m_referenceBuffer = nullptr;
  }

  return *this;
}

//---------------------------------------------------------------------------------------

GenericRenderAction::GenericRenderAction(
  const RawDataView& bufferData,
  const ShaderAttributes& shaderAttributes)
{
  Create(bufferData, shaderAttributes);
}

//---------------------------------------------------------------------------------------

GenericRenderAction::~GenericRenderAction()
{
  Clear();
}

//---------------------------------------------------------------------------------------

void
GenericRenderAction::Create(const GenericVertexBuffer& reusableVertexBuffer,
                            const ShaderAttributes& shaderAttributes)
{
  Clear();

  m_referenceBuffer = &reusableVertexBuffer;
  Configure(reusableVertexBuffer, shaderAttributes);
}

//---------------------------------------------------------------------------------------

void
GenericRenderAction::Create(const RawDataView& vertexData,
                            const ShaderAttributes& shaderAttributes)
{
  Clear();

  const size_t nVertices =
    CalculateNumberOfVertices(vertexData, shaderAttributes);

  m_ownBuffer.reset(new GenericVertexBuffer);
  m_ownBuffer->Create(vertexData, nVertices);

  Configure(*m_ownBuffer, shaderAttributes);
}

//---------------------------------------------------------------------------------------

void
GenericRenderAction::Create(const RawDataView& vertexData,
                            const RawArrayView& indexData,
                            const ShaderAttributes& shaderAttributes)
{
  Clear();

  const size_t nVertices =
    CalculateNumberOfVertices(vertexData, shaderAttributes);

  m_ownBuffer.reset(new GenericVertexBuffer);
  m_ownBuffer->Create(vertexData, nVertices, indexData);

  Configure(*m_ownBuffer, shaderAttributes);
}

//---------------------------------------------------------------------------------------

size_t
GenericRenderAction::CalculateNumberOfVertices(
  const RawDataView& vertexData,
  const ShaderAttributes& shaderAttributes) const
{
  const size_t vertexDataSize = vertexData.size;
  const size_t sizePerVertex =
    BufferUtils::ComputeTotalSizeOfAttributes(shaderAttributes);
  return vertexDataSize / sizePerVertex;
}

//---------------------------------------------------------------------------------------

void
GenericRenderAction::Configure(const GenericVertexBuffer& buffer,
                               const ShaderAttributes& shaderAttributes)
{
  glGenVertexArrays(1, &m_VAO);
  glBindVertexArray(m_VAO);

  BufferPrivate::Bind(buffer);
  BufferUtils::LinkShaderAttributes(shaderAttributes);
}

//---------------------------------------------------------------------------------------

void
GenericRenderAction::LoadVertexData(const RawDataView& vertexData)
{
  if (m_VAO == 0 || m_ownBuffer == nullptr) {
    return;
  }

  m_ownBuffer->LoadVertexData(vertexData);
}

//---------------------------------------------------------------------------------------

void
GenericRenderAction::LoadIndexData(const RawArrayView& indexData)
{
  if (m_VAO == 0 || m_ownBuffer == nullptr) {
    return;
  }

  m_ownBuffer->LoadIndexData(indexData);
}

//---------------------------------------------------------------------------------------

void
GenericRenderAction::Clear()
{
  if (m_VAO != 0) {
    glDeleteVertexArrays(1, &m_VAO);
    m_VAO = 0;
  }

  m_ownBuffer.reset(nullptr);
  m_referenceBuffer = nullptr;
}

//---------------------------------------------------------------------------------------

void
GenericRenderAction::Render() const
{
  if (m_VAO == 0) {
    return;
  }

  const GenericVertexBuffer& buffer =
    m_ownBuffer ? *m_ownBuffer : *m_referenceBuffer;

  glBindVertexArray(m_VAO);
  if (BufferPrivate::HasIndices(buffer)) {
    const unsigned nElements = BufferPrivate::GetNElements(buffer);
    glDrawElements(GL_TRIANGLES, nElements, GL_UNSIGNED_INT, 0);
  } else {
    const unsigned nVertices = BufferPrivate::GetNVertices(buffer);
    glDrawArrays(GL_TRIANGLES, 0, nVertices);
  }
}

//---------------------------------------------------------------------------------------
