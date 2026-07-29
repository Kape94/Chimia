#pragma once

#include "Core/ClassDefs.h"
#include "IndexData.h"
#include "InstancedData.h"
#include "Rendering/IDataChangeListener.h"
#include "RenderingNamespaceDefs.h"
#include "ShaderAttribute.h"
#include "ShaderBinding.h"
#include "VertexData.h"

//---------------------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

class GenericRenderAction : public IDataChangeListener
{
public:
  DEFAULT_CONSTUCTIBLE(GenericRenderAction)
  NON_COPYABLE(GenericRenderAction)

  GenericRenderAction(GenericRenderAction&& other) noexcept;
  GenericRenderAction& operator=(GenericRenderAction&& other) noexcept;

  ~GenericRenderAction();

  void Create(const ShaderBindings& bindings);

  void Create(const ShaderBindings& bindings,
              const IndexDataInstance& indexData);

  void Create(const VertexDataInstance& reusableVertexBuffer,
              const ShaderAttributes& shaderAttributes);

  void Create(const VertexDataInstance& reusableVertexBuffer,
              const IndexDataInstance& reusableIndexBuffer,
              const ShaderAttributes& shaderAttributes);

  void CreateInstanced(const VertexDataInstance& reusableVertexBuffer,
                       const ShaderAttributes& shaderAttributes,
                       const InstancedDataInstance& instancesData,
                       const ShaderAttributes& instanceShaderAttributes);

  void CreateInstanced(const VertexDataInstance& reusableVertexBuffer,
                       const IndexDataInstance& reusableIndexBuffer,
                       const ShaderAttributes& shaderAttributes,
                       const InstancedDataInstance& instancesData,
                       const ShaderAttributes& instanceShaderAttributes);

  void Clear();

  void Render() const;

private:
  void SetupVAO();

  void ClearRenderingData();

  void CollectDatasFromBindings(const ShaderBindings& bindings);

  void RegisterAsListener();
  void UnregisterAsListener();

  void GenerateBindingsAndCreate(const VertexDataInstance& vertexData,
                                 const IndexDataInstance& indexData,
                                 const ShaderAttributes& vertexAttributes,
                                 const InstancedDataInstance& instancedData,
                                 const ShaderAttributes& instancedAttributes);

  void RenderInstanced() const;

  void RenderSingle() const;

  unsigned PickReferenceVertexCount() const;
  unsigned PickReferenceInstanceCount() const;

  void DataChanged() override;

  unsigned m_VAO = 0;

  std::vector<VertexDataInstance> m_referenceVertexDatas;
  IndexDataInstance m_referenceIndexBuffer = nullptr;
  std::vector<InstancedDataInstance> m_referenceInstancedDatas;

  ShaderBindings m_bindings;
};

//---------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------