#include "RenderAction.h"

#include "BufferPrivate.h"
#include "BufferUtils.h"
#include "GLState.h"
#include "IndexData.h"
#include "InstancedData.h"
#include "OpenGLDefs.h"
#include "ShaderBinding.h"
#include "VertexData.h"
#include <set>

//---------------------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

RenderAction::RenderAction(RenderAction&& other) noexcept
  : m_VAO(other.m_VAO)
  , m_referenceVertexDatas(std::move(other.m_referenceVertexDatas))
  , m_referenceIndexBuffer(std::move(other.m_referenceIndexBuffer))
  , m_referenceInstancedDatas(std::move(other.m_referenceInstancedDatas))
  , m_bindings(std::move(other.m_bindings))
{
  other.m_VAO = 0;
}

//---------------------------------------------------------------------------------------

RenderAction&
RenderAction::operator=(RenderAction&& other) noexcept
{
  if (&other != this) {
    m_VAO = other.m_VAO;
    m_referenceVertexDatas = std::move(other.m_referenceVertexDatas);
    m_referenceIndexBuffer = std::move(other.m_referenceIndexBuffer);
    m_referenceInstancedDatas = std::move(m_referenceInstancedDatas);
    m_bindings = std::move(other.m_bindings);

    other.m_VAO = 0;
  }

  return *this;
}

//---------------------------------------------------------------------------------------

RenderAction::~RenderAction()
{
  Clear();
}

//---------------------------------------------------------------------------------------

void
RenderAction::Create(const ShaderBindings& bindings)
{
  Clear();

  CollectDatasFromBindings(bindings);
  RegisterAsListener();

  SetupVAO();
  for (const ShaderBinding& binding : bindings) {
    BufferUtils::LinkShaderBinding(binding);
  }

  m_bindings = bindings;
}

//---------------------------------------------------------------------------------------

void
RenderAction::Create(const ShaderBindings& bindings,
                     const IndexDataInstance& indexData)
{
  Clear();

  CollectDatasFromBindings(bindings);
  RegisterAsListener();

  SetupVAO();
  for (const ShaderBinding& binding : bindings) {
    BufferUtils::LinkShaderBinding(binding);
  }

  m_referenceIndexBuffer = indexData;
  BufferPrivate::Bind(indexData);

  m_bindings = bindings;
}

//---------------------------------------------------------------------------------------

void
RenderAction::SetupVAO()
{
  glGenVertexArrays(1, &m_VAO);
  GLState::BindVertexArray(m_VAO);
}

//---------------------------------------------------------------------------------------

void
RenderAction::CollectDatasFromBindings(const ShaderBindings& bindings)
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
RenderAction::Clear()
{
  UnregisterAsListener();
  ClearRenderingData();
}

//---------------------------------------------------------------------------------------

void
RenderAction::ClearRenderingData()
{
  if (m_VAO != 0) {
    glDeleteVertexArrays(1, &m_VAO);
    m_VAO = 0;
  }

  m_referenceVertexDatas.clear();
  m_referenceIndexBuffer = nullptr;
  m_referenceInstancedDatas.clear();
  m_bindings.Clear();
}

//---------------------------------------------------------------------------------------

void
RenderAction::RegisterAsListener()
{
  for (VertexDataInstance& data : m_referenceVertexDatas) {
    BufferPrivate::AddListener(data, this);
  }
  if (m_referenceIndexBuffer != nullptr) {
    BufferPrivate::AddListener(m_referenceIndexBuffer, this);
  }
  for (InstancedDataInstance& data : m_referenceInstancedDatas) {
    BufferPrivate::AddListener(data, this);
  }
}

//---------------------------------------------------------------------------------------

void
RenderAction::UnregisterAsListener()
{
  for (VertexDataInstance& data : m_referenceVertexDatas) {
    BufferPrivate::RemoveListener(data, this);
  }
  if (m_referenceIndexBuffer != nullptr) {
    BufferPrivate::RemoveListener(m_referenceIndexBuffer, this);
  }
  for (InstancedDataInstance& data : m_referenceInstancedDatas) {
    BufferPrivate::RemoveListener(data, this);
  }
}

//---------------------------------------------------------------------------------------

void
RenderAction::Render() const
{
  if (m_VAO == 0) {
    return;
  }

  GLState::BindVertexArray(m_VAO);

  const bool isInstanced = !m_referenceInstancedDatas.empty();
  if (isInstanced) {
    RenderInstanced();
  } else {
    RenderSingle();
  }
}

//---------------------------------------------------------------------------------------

void
RenderAction::RenderInstanced() const
{
  const bool isIndexed = m_referenceIndexBuffer != nullptr;
  const unsigned nInstances = PickReferenceInstanceCount();
  if (isIndexed) {
    const unsigned nElements =
      BufferPrivate::GetNIndices(m_referenceIndexBuffer);
    glDrawElementsInstanced(
      GL_TRIANGLES, nElements, GL_UNSIGNED_INT, 0, nInstances);
  } else {
    const unsigned nVertices = PickReferenceVertexCount();
    glDrawArraysInstanced(GL_TRIANGLES, 0, nVertices, nInstances);
  }
}

//---------------------------------------------------------------------------------------

void
RenderAction::RenderSingle() const
{
  const bool isIndexed = m_referenceIndexBuffer != nullptr;
  if (isIndexed) {
    const unsigned nElements =
      BufferPrivate::GetNIndices(m_referenceIndexBuffer);
    glDrawElements(GL_TRIANGLES, nElements, GL_UNSIGNED_INT, 0);
  } else {
    const unsigned nVertices = PickReferenceVertexCount();
    glDrawArrays(GL_TRIANGLES, 0, nVertices);
  }
}

//---------------------------------------------------------------------------------------

unsigned
RenderAction::PickReferenceVertexCount() const
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
RenderAction::PickReferenceInstanceCount() const
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

void
RenderAction::DataChanged()
{
  ShaderBindings backupBindings = m_bindings;
  const IndexDataInstance backupIndex = m_referenceIndexBuffer;

  // ClearRenderingData();
  Clear();

  if (backupIndex != nullptr) {
    Create(backupBindings, backupIndex);
  } else {
    Create(backupBindings);
  }
}

//---------------------------------------------------------------------------------------