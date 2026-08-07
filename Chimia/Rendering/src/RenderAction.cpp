#include "RenderAction.h"

#include "BufferPrivate.h"
#include "BufferUtils.h"
#include "GLState.h"
#include "IDataChangeListener.h"
#include "IndexData.h"
#include "InstancedData.h"
#include "OpenGLDefs.h"
#include "Shader.h"
#include "ShaderBinding.h"
#include "Target.h"
#include "VertexData.h"
#include <set>
#include <vector>

//---------------------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------
// RenderAction::Binding
//---------------------------------------------------------------------------------------

RenderAction::Binding::Binding(const VertexDataInstance& data,
                               const std::string& sourceData,
                               const std::string& shaderInput)
  : m_vertexData(data)
  , m_instancedData(nullptr)
  , m_data(sourceData)
  , m_shaderInput(shaderInput)
{
}

//---------------------------------------------------------------------------------------

RenderAction::Binding::Binding(const InstancedDataInstance& data,
                               const std::string& sourceData,
                               const std::string& shaderInput)
  : m_vertexData(nullptr)
  , m_instancedData(data)
  , m_data(sourceData)
  , m_shaderInput(shaderInput)
{
}

//---------------------------------------------------------------------------------------

RenderAction::Binding::Binding(const Binding& other)
  : m_vertexData(other.m_vertexData)
  , m_instancedData(other.m_instancedData)
  , m_data(other.m_data)
  , m_shaderInput(other.m_shaderInput)
{
}

//---------------------------------------------------------------------------------------

RenderAction::Binding&
RenderAction::Binding::operator=(const Binding& other)
{
  m_vertexData = other.m_vertexData;
  m_instancedData = other.m_instancedData;
  m_data = other.m_data;
  m_shaderInput = other.m_shaderInput;

  return *this;
}

//---------------------------------------------------------------------------------------

const VertexDataInstance&
RenderAction::Binding::GetVertexData() const
{
  return m_vertexData;
}

//---------------------------------------------------------------------------------------

const InstancedDataInstance&
RenderAction::Binding::GetInstancedData() const
{
  return m_instancedData;
}

//---------------------------------------------------------------------------------------

const std::string&
RenderAction::Binding::GetInputDataName() const
{
  return m_data;
}

//---------------------------------------------------------------------------------------

const std::string&
RenderAction::Binding::GetShaderInputName() const
{
  return m_shaderInput;
}

//---------------------------------------------------------------------------------------
// RenderAction::Listener
//---------------------------------------------------------------------------------------

class RenderAction::Listener : public IDataChangeListener
{
public:
  Listener(RenderAction& action)
    : m_action(action)
  {
  }

  void DataChanged() override { m_action.DataChanged(); }

private:
  RenderAction& m_action;
};

//---------------------------------------------------------------------------------------
// RenderActionInternal
//---------------------------------------------------------------------------------------

namespace RenderActionInternal {
void
CollectDatasFromBindings(const ShaderBindings& bindings,
                         std::vector<VertexDataInstance>& vertexDatas,
                         std::vector<InstancedDataInstance>& instancedDatas)
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
    vertexDatas.push_back(vertexData);
  }
  for (const InstancedDataInstance& instancedData : instancedSet) {
    instancedDatas.push_back(instancedData);
  }
}

ShaderBindings
GenerateDetailedBindings(const ShaderInstance& shader,
                         const std::vector<RenderAction::Binding>& bindings)
{
  ShaderBindings detailedBindings;

  for (const auto& binding : bindings) {
    if (const VertexDataInstance vertexData = binding.GetVertexData()) {
      detailedBindings.Insert(
        ShaderBinding::Connect(vertexData,
                               binding.GetInputDataName(),
                               shader,
                               binding.GetShaderInputName()));
    } else {
      detailedBindings.Insert(
        ShaderBinding::Connect(binding.GetInstancedData(),
                               binding.GetInputDataName(),
                               shader,
                               binding.GetShaderInputName()));
    }
  }

  return detailedBindings;
}
}

//---------------------------------------------------------------------------------------
// RenderAction
//---------------------------------------------------------------------------------------

RenderAction::RenderAction()
  : m_dataListener(new Listener(*this))
{
}

//---------------------------------------------------------------------------------------

RenderAction::RenderAction(RenderAction&& other) noexcept
  : m_VAO(other.m_VAO)
  , m_referenceVertexDatas(std::move(other.m_referenceVertexDatas))
  , m_referenceIndexBuffer(std::move(other.m_referenceIndexBuffer))
  , m_referenceInstancedDatas(std::move(other.m_referenceInstancedDatas))
  , m_target(std::move(other.m_target))
  , m_bindings(std::move(other.m_bindings))
  , m_dataListener(std::move(other.m_dataListener))
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
    m_target = std::move(other.m_target);
    m_bindings = std::move(other.m_bindings);
    m_dataListener = std::move(other.m_dataListener);

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
RenderAction::Create(const TargetInstance& target,
                     const std::vector<Binding>& bindings)
{
  Clear();

  Setup(target, nullptr /*indexData*/, bindings);
}

//---------------------------------------------------------------------------------------

void
RenderAction::Create(const TargetInstance& target,
                     const IndexDataInstance& indexData,
                     const std::vector<Binding>& bindings)
{
  Clear();

  Setup(target, indexData, bindings);
}

//---------------------------------------------------------------------------------------

void
RenderAction::Setup(const TargetInstance& target,
                    const IndexDataInstance& indexData,
                    const std::vector<Binding>& bindings)
{
  const ShaderBindings detailedBindings =
    RenderActionInternal::GenerateDetailedBindings(
      BufferPrivate::GetShader(target), bindings);

  RenderActionInternal::CollectDatasFromBindings(
    detailedBindings, m_referenceVertexDatas, m_referenceInstancedDatas);
  RegisterAsListener();

  SetupVAO();
  for (const ShaderBinding& binding : detailedBindings) {
    BufferUtils::LinkShaderBinding(binding);
  }

  if (indexData != nullptr) {
    m_referenceIndexBuffer = indexData;
    BufferPrivate::Bind(indexData);
  }

  m_bindings = bindings;
  m_target = target;
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
RenderAction::Clear()
{
  if (m_VAO != 0) {
    glDeleteVertexArrays(1, &m_VAO);
    m_VAO = 0;
  }

  UnregisterAsListener();
  m_referenceVertexDatas.clear();
  m_referenceIndexBuffer = nullptr;
  m_referenceInstancedDatas.clear();
  m_bindings.clear();
  m_target = nullptr;
}

//---------------------------------------------------------------------------------------

void
RenderAction::RegisterAsListener()
{
  Listener* listener = this->m_dataListener.get();

  for (VertexDataInstance& data : m_referenceVertexDatas) {
    BufferPrivate::AddListener(data, listener);
  }
  if (m_referenceIndexBuffer != nullptr) {
    BufferPrivate::AddListener(m_referenceIndexBuffer, listener);
  }
  for (InstancedDataInstance& data : m_referenceInstancedDatas) {
    BufferPrivate::AddListener(data, listener);
  }
}

//---------------------------------------------------------------------------------------

void
RenderAction::UnregisterAsListener()
{
  Listener* listener = this->m_dataListener.get();

  for (VertexDataInstance& data : m_referenceVertexDatas) {
    BufferPrivate::RemoveListener(data, listener);
  }
  if (m_referenceIndexBuffer != nullptr) {
    BufferPrivate::RemoveListener(m_referenceIndexBuffer, listener);
  }
  for (InstancedDataInstance& data : m_referenceInstancedDatas) {
    BufferPrivate::RemoveListener(data, listener);
  }
}

//---------------------------------------------------------------------------------------

void
RenderAction::Retarget(const TargetInstance& target)
{
  const std::vector<Binding> backupBindings = m_bindings;
  const IndexDataInstance backupIndex = m_referenceIndexBuffer;

  Create(target, backupIndex, backupBindings);
}

//---------------------------------------------------------------------------------------

void
RenderAction::Render() const
{
  if (m_VAO == 0) {
    return;
  }

  BufferPrivate::UseShader(BufferPrivate::GetShader(m_target));
  BufferPrivate::UseFramebuffer(
    BufferPrivate::BufferPrivate::GetFramebuffer(m_target));

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
  const std::vector<Binding> backupBindings = m_bindings;
  const IndexDataInstance backupIndex = m_referenceIndexBuffer;
  const TargetInstance backupTarget = m_target;

  Clear();

  if (backupIndex != nullptr) {
    Create(backupTarget, backupIndex, backupBindings);
  } else {
    Create(backupTarget, backupBindings);
  }
}

//---------------------------------------------------------------------------------------