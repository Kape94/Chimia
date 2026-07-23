#include "GenericRenderAction.h"

#include "BufferPrivate.h"
#include "BufferUtils.h"
#include "Core/Types.h"
#include "GLState.h"
#include "IndexData.h"
#include "InstancedData.h"
#include "OpenGLDefs.h"
#include "ShaderAttribute.h"
#include "ShaderBinding.h"
#include "VertexData.h"
#include <set>

//---------------------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

GenericRenderAction::GenericRenderAction(GenericRenderAction&& other) noexcept
  : m_VAO(other.m_VAO)
  , m_ownBuffer(std::move(other.m_ownBuffer))
  , m_referenceVertexDatas(std::move(other.m_referenceVertexDatas))
  , m_ownIndexBuffer(std::move(other.m_ownIndexBuffer))
  , m_referenceIndexBuffer(std::move(other.m_referenceIndexBuffer))
  , m_instancedBuffer(std::move(other.m_instancedBuffer))
  , m_referenceInstancedDatas(std::move(other.m_referenceInstancedDatas))
{
  other.m_VAO = 0;
}

//---------------------------------------------------------------------------------------

GenericRenderAction&
GenericRenderAction::operator=(GenericRenderAction&& other) noexcept
{
  if (&other != this) {
    m_VAO = other.m_VAO;
    m_ownBuffer = std::move(other.m_ownBuffer);
    m_referenceVertexDatas = std::move(other.m_referenceVertexDatas);
    m_ownIndexBuffer = std::move(other.m_ownIndexBuffer);
    m_referenceIndexBuffer = std::move(other.m_referenceIndexBuffer);
    m_instancedBuffer = std::move(m_instancedBuffer);
    m_referenceInstancedDatas = std::move(m_referenceInstancedDatas);

    other.m_VAO = 0;
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
GenericRenderAction::Create(const std::vector<ShaderBinding>& bindings)
{
  Clear();

  CollectDatasFromBindings(bindings);

  SetupVAO();
  for (const ShaderBinding& binding : bindings) {
    BufferUtils::LinkShaderBinding(binding);
  }
}

//---------------------------------------------------------------------------------------

void
GenericRenderAction::Create(const std::vector<ShaderBinding>& bindings,
                            const IndexDataInstance& indexData)
{
  Clear();

  CollectDatasFromBindings(bindings);

  SetupVAO();
  for (const ShaderBinding& binding : bindings) {
    BufferUtils::LinkShaderBinding(binding);
  }

  m_referenceIndexBuffer = indexData;
  BufferPrivate::Bind(indexData);
}

//---------------------------------------------------------------------------------------

void
GenericRenderAction::Create(const VertexDataInstance& reusableVertexBuffer,
                            const ShaderAttributes& shaderAttributes)
{
  Clear();

  m_referenceVertexDatas.push_back(reusableVertexBuffer);

  SetupVAO();
  Configure(reusableVertexBuffer, nullptr /*indexData*/, shaderAttributes);
}

//---------------------------------------------------------------------------------------

void
GenericRenderAction::Create(const VertexDataInstance& reusableVertexBuffer,
                            const IndexDataInstance& reusableIndexBuffer,
                            const ShaderAttributes& shaderAttributes)
{
  Clear();

  m_referenceVertexDatas.push_back(reusableVertexBuffer);
  m_referenceIndexBuffer = reusableIndexBuffer;

  SetupVAO();
  Configure(reusableVertexBuffer, reusableIndexBuffer, shaderAttributes);
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
  const VertexDataInstance& reusableVertexBuffer,
  const ShaderAttributes& shaderAttributes,
  const RawArrayView& instancesData,
  const ShaderAttributes& instanceShaderAttributes)
{
  Clear();

  m_referenceVertexDatas.push_back(reusableVertexBuffer);

  SetupVAO();
  Configure(reusableVertexBuffer, nullptr /*indexData*/, shaderAttributes);
  SetupOwnInstancedBuffer(instancesData, instanceShaderAttributes);
}

//---------------------------------------------------------------------------------------

void
GenericRenderAction::CreateInstanced(
  const VertexDataInstance& reusableVertexBuffer,
  const IndexDataInstance& reusableIndexBuffer,
  const ShaderAttributes& shaderAttributes,
  const RawArrayView& instancesData,
  const ShaderAttributes& instanceShaderAttributes)
{
  Clear();

  m_referenceVertexDatas.push_back(reusableVertexBuffer);
  m_referenceIndexBuffer = reusableIndexBuffer;

  SetupVAO();
  Configure(reusableVertexBuffer, reusableIndexBuffer, shaderAttributes);
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
  GLState::BindVertexArray(m_VAO);
}

//---------------------------------------------------------------------------------------

void
GenericRenderAction::CollectDatasFromBindings(
  const std::vector<ShaderBinding>& bindings)
{
  std::set<VertexDataInstance> vertexSet;
  std::set<InstancedDataInstance> instancedSet;

  for (const ShaderBinding& binding : bindings) {
    if (const VertexDataInstance data = BufferPrivate::GetVertexData(binding)) {
      vertexSet.insert(data);
    }
    if (const InstancedDataInstance instancedData =
          BufferPrivate::GetInstancedData(binding)) {
      instancedSet.insert(instancedData);
    }
  }

  for (const VertexDataInstance& vertexData : vertexSet) {
    m_referenceVertexDatas.push_back(vertexData);
  }
  for (const InstancedDataInstance& instancedData : instancedSet) {
    m_referenceInstancedDatas.push_back(instancedData);
  }
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

  m_ownBuffer = VertexData::New();
  m_ownBuffer->Create(vertexData, nVertices);
  m_referenceVertexDatas.push_back(m_ownBuffer);

  if (optionalIndexData != nullptr) {
    m_ownIndexBuffer = IndexData::New();
    m_ownIndexBuffer->Create(*optionalIndexData);
  }

  Configure(m_ownBuffer, m_ownIndexBuffer, shaderAttributes);
}

//---------------------------------------------------------------------------------------

void
GenericRenderAction::SetupOwnInstancedBuffer(
  const RawArrayView& instancesData,
  const ShaderAttributes& instanceShaderAttributes)
{
  m_instancedBuffer = InstancedData::New();
  m_instancedBuffer->Create(instancesData);
  m_referenceInstancedDatas.push_back(m_instancedBuffer);

  BufferUtils::LinkShaderAttributes(instanceShaderAttributes,
                                    m_instancedBuffer);
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
GenericRenderAction::Configure(const VertexDataInstance& buffer,
                               const IndexDataInstance& indexData,
                               const ShaderAttributes& shaderAttributes)
{
  BufferUtils::LinkShaderAttributes(shaderAttributes, buffer);

  if (indexData != nullptr) {
    BufferPrivate::Bind(indexData);
  }
}

//---------------------------------------------------------------------------------------

void
GenericRenderAction::LoadVertexData(const RawDataView& vertexData)
{
  if (m_VAO == 0 || m_ownBuffer == nullptr) {
    return;
  }

  m_ownBuffer->Load(vertexData);
}

//---------------------------------------------------------------------------------------

void
GenericRenderAction::LoadIndexData(const RawArrayView& indexData)
{
  if (m_VAO == 0 || m_ownIndexBuffer == nullptr) {
    return;
  }

  m_ownIndexBuffer->LoadIndexData(indexData);
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
  GLState::BindVertexArray(m_VAO);
  InstancedDataInstance oldBuffer = std::move(m_instancedBuffer);
  m_referenceInstancedDatas.erase(std::remove(m_referenceInstancedDatas.begin(),
                                              m_referenceInstancedDatas.end(),
                                              oldBuffer),
                                  m_referenceInstancedDatas.end());

  SetupOwnInstancedBuffer(instancesData, instanceShaderAttributes);

  oldBuffer->Clear();

  GLState::BindVertexArray(0);
}

//---------------------------------------------------------------------------------------

void
GenericRenderAction::Clear()
{
  if (m_VAO != 0) {
    glDeleteVertexArrays(1, &m_VAO);
    m_VAO = 0;
  }

  m_ownBuffer = nullptr;
  m_referenceVertexDatas.clear();

  m_ownIndexBuffer = nullptr;
  m_referenceIndexBuffer = nullptr;

  m_instancedBuffer = nullptr;
  m_referenceInstancedDatas.clear();
}

//---------------------------------------------------------------------------------------

void
GenericRenderAction::Render() const
{
  if (m_VAO == 0) {
    return;
  }

  const IndexDataInstance indexData = m_referenceIndexBuffer != nullptr
                                        ? m_referenceIndexBuffer
                                        : m_ownIndexBuffer;

  GLState::BindVertexArray(m_VAO);

  const bool isInstanced = !m_referenceInstancedDatas.empty();
  if (isInstanced) {
    RenderInstanced(indexData);
  } else {
    RenderSingle(indexData);
  }
}

//---------------------------------------------------------------------------------------

void
GenericRenderAction::RenderInstanced(const IndexDataInstance& indexData) const
{
  const bool isIndexed = indexData != nullptr;
  const unsigned nInstances = PickReferenceInstanceCount();
  if (isIndexed) {
    const unsigned nElements = BufferPrivate::GetNIndices(indexData);
    glDrawElementsInstanced(
      GL_TRIANGLES, nElements, GL_UNSIGNED_INT, 0, nInstances);
  } else {
    const unsigned nVertices = PickReferenceVertexCount();
    glDrawArraysInstanced(GL_TRIANGLES, 0, nVertices, nInstances);
  }
}

//---------------------------------------------------------------------------------------

void
GenericRenderAction::RenderSingle(const IndexDataInstance& indexData) const
{
  const bool isIndexed = indexData != nullptr;
  if (isIndexed) {
    const unsigned nElements = BufferPrivate::GetNIndices(indexData);
    glDrawElements(GL_TRIANGLES, nElements, GL_UNSIGNED_INT, 0);
  } else {
    const unsigned nVertices = PickReferenceVertexCount();
    glDrawArrays(GL_TRIANGLES, 0, nVertices);
  }
}

//---------------------------------------------------------------------------------------

unsigned
GenericRenderAction::PickReferenceVertexCount() const
{
  unsigned nVertices =
    BufferPrivate::GetNVertices(m_referenceVertexDatas.front());

  const size_t nDatas = m_referenceVertexDatas.size();
  for (size_t i = 1; i < nDatas; ++i) {
    const unsigned current =
      BufferPrivate::GetNVertices(m_referenceVertexDatas[i]);
    if (current < nVertices) {
      nVertices = current;
    }
  }

  return nVertices;
}

//---------------------------------------------------------------------------------------

unsigned
GenericRenderAction::PickReferenceInstanceCount() const
{
  unsigned nInstances =
    BufferPrivate::GetNInstances(m_referenceInstancedDatas.front());

  const size_t nDatas = m_referenceInstancedDatas.size();
  for (size_t i = 1; i < nDatas; ++i) {
    const unsigned current =
      BufferPrivate::GetNInstances(m_referenceInstancedDatas[i]);
    if (current < nInstances) {
      nInstances = current;
    }
  }

  return nInstances;
}

//---------------------------------------------------------------------------------------