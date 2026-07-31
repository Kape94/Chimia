#pragma once

#include "Core/ClassDefs.h"
#include "IndexData.h"
#include "InstancedData.h"
#include "Rendering/IDataChangeListener.h"
#include "RenderingNamespaceDefs.h"
#include "Shader.h"
#include "ShaderBinding.h"
#include "VertexData.h"
#include <initializer_list>
#include <vector>

//---------------------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

class RenderAction : public IDataChangeListener
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

  private:
    friend class RenderAction;

    VertexDataInstance m_vertexData = nullptr;
    InstancedDataInstance m_instancedData = nullptr;
    std::string m_data;
    std::string m_shaderInput;
  };
  // ----------------------------------------------------------------

  DEFAULT_CONSTUCTIBLE(RenderAction)
  NON_COPYABLE(RenderAction)

  RenderAction(RenderAction&& other) noexcept;
  RenderAction& operator=(RenderAction&& other) noexcept;

  ~RenderAction();

  void Create(const Shader& shader, const std::vector<Binding>& bindings);

  void Create(const Shader& shader,
              const IndexDataInstance& indexData,
              const std::vector<Binding>& bindings);

  void Clear();

  void Render() const;

private:
  void Create(const ShaderBindings& bindings);

  void Create(const ShaderBindings& bindings,
              const IndexDataInstance& indexData);

  void SetupVAO();

  void ClearRenderingData();

  void CollectDatasFromBindings(const ShaderBindings& bindings);

  void RegisterAsListener();
  void UnregisterAsListener();

  void RenderInstanced() const;

  void RenderSingle() const;

  unsigned PickReferenceVertexCount() const;
  unsigned PickReferenceInstanceCount() const;

  void DataChanged() override;

  unsigned m_VAO = 0;

  std::vector<VertexDataInstance> m_referenceVertexDatas;
  IndexDataInstance m_referenceIndexBuffer = nullptr;
  std::vector<InstancedDataInstance> m_referenceInstancedDatas;

  const Shader* m_shader = nullptr;
  std::vector<Binding> m_bindings;
};

//---------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------