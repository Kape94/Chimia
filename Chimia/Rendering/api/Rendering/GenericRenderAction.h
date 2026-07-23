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

  GenericRenderAction(const RawDataView& vertexData,
                      const ShaderAttributes& shaderAttributes);

  ~GenericRenderAction();

  void Create(const std::vector<ShaderBinding>& bindings);

  void Create(const std::vector<ShaderBinding>& bindings,
              const IndexDataInstance& indexData);

  void Create(const VertexDataInstance& reusableVertexBuffer,
              const ShaderAttributes& shaderAttributes);

  void Create(const VertexDataInstance& reusableVertexBuffer,
              const IndexDataInstance& reusableIndexBuffer,
              const ShaderAttributes& shaderAttributes);

  void Create(const RawDataView& vertexData,
              const ShaderAttributes& shaderAttributes);

  void Create(const RawDataView& vertexData,
              const RawArrayView& indexData,
              const ShaderAttributes& shaderAttributes);

  void CreateInstanced(const VertexDataInstance& reusableVertexBuffer,
                       const ShaderAttributes& shaderAttributes,
                       const RawArrayView& instancesData,
                       const ShaderAttributes& instanceShaderAttributes);

  void CreateInstanced(const VertexDataInstance& reusableVertexBuffer,
                       const IndexDataInstance& reusableIndexBuffer,
                       const ShaderAttributes& shaderAttributes,
                       const RawArrayView& instancesData,
                       const ShaderAttributes& instanceShaderAttributes);

  void CreateInstanced(const RawDataView& vertexData,
                       const ShaderAttributes& shaderAttributes,
                       const RawArrayView& instancesData,
                       const ShaderAttributes& instanceShaderAttributes);

  void CreateInstanced(const RawDataView& vertexData,
                       const RawArrayView& indexData,
                       const ShaderAttributes& shaderAttributes,
                       const RawArrayView& instancesData,
                       const ShaderAttributes& instanceShaderAttributes);

  void LoadVertexData(const RawDataView& vertexData);
  void LoadIndexData(const RawArrayView& indexData);
  void LoadInstancedData(const RawArrayView& instancesData);

  void RecreateInstancedBuffer(
    const RawArrayView& instancesData,
    const ShaderAttributes& instanceShaderAttributes);

  void Clear();

  void Render() const;

private:
  void SetupOwnVertexBuffer(const RawDataView& vertexData,
                            const RawArrayView* optionalIndexData,
                            const ShaderAttributes& shaderAttributes);

  void SetupOwnInstancedBuffer(
    const RawArrayView& instancesData,
    const ShaderAttributes& instanceShaderAttributes);

  size_t CalculateNumberOfVertices(
    const RawDataView& vertexData,
    const ShaderAttributes& shaderAttributes) const;

  void SetupVAO();

  void CollectDatasFromBindings(const std::vector<ShaderBinding>& bindings);

  void Configure(const VertexDataInstance& buffer,
                 const IndexDataInstance& indexData,
                 const ShaderAttributes& shaderAttributes);

  void RenderInstanced(const IndexDataInstance& indexData) const;

  void RenderSingle(const IndexDataInstance& indexData) const;

  unsigned PickReferenceVertexCount() const;
  unsigned PickReferenceInstanceCount() const;

  friend class BufferPrivate;

  unsigned m_VAO = 0;

  VertexDataInstance m_ownBuffer = nullptr;
  std::vector<VertexDataInstance> m_referenceVertexDatas;

  IndexDataInstance m_ownIndexBuffer = nullptr;
  IndexDataInstance m_referenceIndexBuffer = nullptr;

  InstancedDataInstance m_instancedBuffer = nullptr;
  std::vector<InstancedDataInstance> m_referenceInstancedDatas;
};

//---------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------