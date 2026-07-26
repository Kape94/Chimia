#pragma once

#include "Core/ClassDefs.h"
#include "IndexData.h"
#include "InstancedData.h"
#include "RenderingNamespaceDefs.h"
#include "ShaderAttribute.h"
#include "ShaderBinding.h"
#include "VertexData.h"

#include "Core/Types.h"

//---------------------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

class GenericRenderAction
{
public:
  DEFAULT_CONSTUCTIBLE(GenericRenderAction)
  NON_COPYABLE(GenericRenderAction)

  GenericRenderAction(GenericRenderAction&& other) noexcept;
  GenericRenderAction& operator=(GenericRenderAction&& other) noexcept;

  ~GenericRenderAction();

  void Create(const std::vector<ShaderBinding>& bindings);

  void Create(const std::vector<ShaderBinding>& bindings,
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

  void RelinkInstancedData(const InstancedDataInstance& instancesData,
                           const ShaderAttributes& instanceShaderAttributes);

  void Clear();

  void Render() const;

private:
  void SetupVAO();

  void CollectDatasFromBindings(const std::vector<ShaderBinding>& bindings);

  void Configure(const VertexDataInstance& buffer,
                 const IndexDataInstance& indexData,
                 const ShaderAttributes& shaderAttributes);

  void RenderInstanced() const;

  void RenderSingle() const;

  unsigned PickReferenceVertexCount() const;
  unsigned PickReferenceInstanceCount() const;

  unsigned m_VAO = 0;

  std::vector<VertexDataInstance> m_referenceVertexDatas;
  IndexDataInstance m_referenceIndexBuffer = nullptr;
  std::vector<InstancedDataInstance> m_referenceInstancedDatas;
};

//---------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------