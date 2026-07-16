#include "GenericRenderAction.h"

#include "BufferPrivate.h"
#include "BufferUtils.h"
#include "Core/Types.h"
#include "InstancedDataBuffer.h"
#include "OpenGLDefs.h"
#include "ShaderAttribute.h"
#include "VertexRenderData.h"

//---------------------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

GenericRenderAction::GenericRenderAction(GenericRenderAction&& other) noexcept
  : m_VAO(other.m_VAO)
  , m_ownBuffer(std::move(other.m_ownBuffer))
  , m_referenceBuffer(other.m_referenceBuffer)
  , m_instancedBuffer(std::move(other.m_instancedBuffer))
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
    m_instancedBuffer = std::move(m_instancedBuffer);

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
GenericRenderAction::Create(const VertexRenderData& reusableVertexBuffer,
                            const ShaderAttributes& shaderAttributes)
{
  Clear();

  m_referenceBuffer = &reusableVertexBuffer;

  SetupVAO();
  Configure(reusableVertexBuffer, shaderAttributes);
}

//---------------------------------------------------------------------------------------

void
GenericRenderAction::Create(const RawDataView& vertexData,
                            const ShaderAttributes& shaderAttributes)
{
  Clear();

  SetupVAO();
  SetupOwnVertexBuffer(
    vertexData, nullptr /*optionalIndexData*/, shaderAttributes);
}

//---------------------------------------------------------------------------------------

void
GenericRenderAction::Create(const RawDataView& vertexData,
                            const RawArrayView& indexData,
                            const ShaderAttributes& shaderAttributes)
{
  Clear();

  SetupVAO();
  SetupOwnVertexBuffer(vertexData, &indexData, shaderAttributes);
}

//---------------------------------------------------------------------------------------

void
GenericRenderAction::CreateInstanced(
  const VertexRenderData& reusableVertexBuffer,
  const ShaderAttributes& shaderAttributes,
  const RawArrayView& instancesData,
  const ShaderAttributes& instanceShaderAttributes)
{
  Clear();

  m_referenceBuffer = &reusableVertexBuffer;

  SetupVAO();
  Configure(reusableVertexBuffer, shaderAttributes);
  SetupOwnInstancedBuffer(instancesData, instanceShaderAttributes);
}

//---------------------------------------------------------------------------------------

void
GenericRenderAction::CreateInstanced(
  const RawDataView& vertexData,
  const RawArrayView& indexData,
  const ShaderAttributes& shaderAttributes,
  const RawArrayView& instancesData,
  const ShaderAttributes& instanceShaderAttributes)
{
  Clear();

  SetupVAO();
  SetupOwnVertexBuffer(vertexData, &indexData, shaderAttributes);
  SetupOwnInstancedBuffer(instancesData, instanceShaderAttributes);
}

//---------------------------------------------------------------------------------------

void
GenericRenderAction::CreateInstanced(
  const RawDataView& vertexData,
  const ShaderAttributes& shaderAttributes,
  const RawArrayView& instancesData,
  const ShaderAttributes& instanceShaderAttributes)
{
  Clear();

  SetupVAO();
  SetupOwnVertexBuffer(vertexData, nullptr /*indexData*/, shaderAttributes);
  SetupOwnInstancedBuffer(instancesData, instanceShaderAttributes);
}

//---------------------------------------------------------------------------------------

void
GenericRenderAction::SetupVAO()
{
  glGenVertexArrays(1, &m_VAO);
  glBindVertexArray(m_VAO);
}

//---------------------------------------------------------------------------------------

void
GenericRenderAction::SetupOwnVertexBuffer(
  const RawDataView& vertexData,
  const RawArrayView* optionalIndexData,
  const ShaderAttributes& shaderAttributes)
{
  const size_t nVertices =
    CalculateNumberOfVertices(vertexData, shaderAttributes);

  m_ownBuffer.reset(new VertexRenderData);
  if (optionalIndexData != nullptr) {
    m_ownBuffer->Create(vertexData, nVertices, *optionalIndexData);
  } else {
    m_ownBuffer->Create(vertexData, nVertices);
  }

  Configure(*m_ownBuffer, shaderAttributes);
}

//---------------------------------------------------------------------------------------

void
GenericRenderAction::SetupOwnInstancedBuffer(
  const RawArrayView& instancesData,
  const ShaderAttributes& instanceShaderAttributes)
{
  m_instancedBuffer.reset(new InstancedDataBuffer);
  m_instancedBuffer->Create(instancesData);

  BufferPrivate::Bind(*m_instancedBuffer);
  BufferUtils::LinkInstancedShaderAttributes(instanceShaderAttributes);
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
GenericRenderAction::Configure(const VertexRenderData& buffer,
                               const ShaderAttributes& shaderAttributes)
{
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
GenericRenderAction::LoadInstancedData(const RawArrayView& instancesData)
{
  if (m_VAO == 0 || m_instancedBuffer == nullptr) {
    return;
  }

  m_instancedBuffer->Load(instancesData);
}

//---------------------------------------------------------------------------------------

void
GenericRenderAction::RecreateInstancedBuffer(
  const RawArrayView& instancesData,
  const ShaderAttributes& instanceShaderAttributes)
{
  if (m_VAO == 0) {
    return;
  }

  /* A quick note: for some reason clearing the buffer and creating a new one
     right after doesn't works. While debugging I noticed that doing that way,
     the ID for the buffer wasn't getting changed, and this was kinda messing
     things in the driver. Doing this way (creating the new buffer first), the
     m_instancedVBO forcefully acquires a different ID, and I think for some
     reason this makes the process work in the graphics driver.
  */
  glBindVertexArray(m_VAO);
  std::unique_ptr<InstancedDataBuffer> oldBuffer = std::move(m_instancedBuffer);

  SetupOwnInstancedBuffer(instancesData, instanceShaderAttributes);

  oldBuffer->Clear();

  glBindVertexArray(0);
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

  m_instancedBuffer.reset(nullptr);
}

//---------------------------------------------------------------------------------------

void
GenericRenderAction::Render() const
{
  if (m_VAO == 0) {
    return;
  }

  const VertexRenderData& buffer =
    m_ownBuffer ? *m_ownBuffer : *m_referenceBuffer;

  glBindVertexArray(m_VAO);

  const bool isInstanced = m_instancedBuffer != nullptr;
  if (isInstanced) {
    RenderInstanced(buffer);
  } else {
    RenderSingle(buffer);
  }
}

//---------------------------------------------------------------------------------------

void
GenericRenderAction::RenderInstanced(const VertexRenderData& buffer) const
{
  const bool isIndexed = BufferPrivate::HasIndices(buffer);
  const unsigned nInstances = BufferPrivate::GetNInstances(*m_instancedBuffer);
  if (isIndexed) {
    const unsigned nElements = BufferPrivate::GetNElements(buffer);
    glDrawElementsInstanced(
      GL_TRIANGLES, nElements, GL_UNSIGNED_INT, 0, nInstances);
  } else {
    const unsigned nVertices = BufferPrivate::GetNVertices(buffer);
    glDrawArraysInstanced(GL_TRIANGLES, 0, nVertices, nInstances);
  }
}

//---------------------------------------------------------------------------------------

void
GenericRenderAction::RenderSingle(const VertexRenderData& buffer) const
{
  const bool isIndexed = BufferPrivate::HasIndices(buffer);
  if (isIndexed) {
    const unsigned nElements = BufferPrivate::GetNElements(buffer);
    glDrawElements(GL_TRIANGLES, nElements, GL_UNSIGNED_INT, 0);
  } else {
    const unsigned nVertices = BufferPrivate::GetNVertices(buffer);
    glDrawArrays(GL_TRIANGLES, 0, nVertices);
  }
}

//---------------------------------------------------------------------------------------
