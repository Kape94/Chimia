#pragma once

#include "Core/ClassDefs.h"
#include "IndexData.h"
#include "InstancedData.h"
#include "RenderingNamespaceDefs.h"
#include "Target.h"
#include "VertexData.h"

#include <vector>

//---------------------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

class RenderAction
{
public:
  // ----------------------------------------------------------------
  class Binding
  {
  public:
    Binding(const VertexDataInstance& data,
            const std::string& sourceData,
            const std::string& shaderInput);

    Binding(const InstancedDataInstance& data,
            const std::string& sourceData,
            const std::string& shaderInput);

    Binding(const Binding& other);
    Binding& operator=(const Binding& other);

    const VertexDataInstance& GetVertexData() const;
    const InstancedDataInstance& GetInstancedData() const;
    const std::string& GetInputDataName() const;
    const std::string& GetShaderInputName() const;

  private:
    friend class RenderAction;

    VertexDataInstance m_vertexData = nullptr;
    InstancedDataInstance m_instancedData = nullptr;
    std::string m_data;
    std::string m_shaderInput;
  };
  // ----------------------------------------------------------------

  NON_COPYABLE(RenderAction)

  RenderAction();

  RenderAction(RenderAction&& other) noexcept;
  RenderAction& operator=(RenderAction&& other) noexcept;

  ~RenderAction();

  void Create(const TargetInstance& target,
              const std::vector<Binding>& bindings);

  void Create(const TargetInstance& target,
              const IndexDataInstance& indexData,
              const std::vector<Binding>& bindings);

  void Render() const;

private:
  void Setup(const TargetInstance& target,
             const IndexDataInstance& indexData,
             const std::vector<Binding>& bindings);

  void SetupVAO();

  void Clear();
  void ClearRenderingData();

  void RegisterAsListener();
  void UnregisterAsListener();

  void RenderInstanced() const;

  void RenderSingle() const;

  unsigned PickReferenceVertexCount() const;
  unsigned PickReferenceInstanceCount() const;

  void DataChanged();

  unsigned m_VAO = 0;

  std::vector<VertexDataInstance> m_referenceVertexDatas;
  IndexDataInstance m_referenceIndexBuffer = nullptr;
  std::vector<InstancedDataInstance> m_referenceInstancedDatas;

  TargetInstance m_target = nullptr;
  std::vector<Binding> m_bindings;

  class Listener;
  std::unique_ptr<Listener> m_dataListener;
};

//---------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------