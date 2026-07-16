#pragma once

#include "Core/ClassDefs.h"
#include "InstancedDataBuffer.h"
#include "RenderingNamespaceDefs.h"

#include "ShaderAttribute.h"
#include "VertexRenderData.h"

#include "Core/Types.h"
#include <memory>

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

  void Create(const VertexRenderData& reusableVertexBuffer,
              const ShaderAttributes& shaderAttributes);

  void Create(const RawDataView& vertexData,
              const ShaderAttributes& shaderAttributes);

  void Create(const RawDataView& vertexData,
              const RawArrayView& indexData,
              const ShaderAttributes& shaderAttributes);

  void CreateInstanced(const VertexRenderData& reusableVertexBuffer,
                       const ShaderAttributes& shaderAttributes,
                       const RawArrayView& instancesData,
                       const ShaderAttributes& instanceShaderAttributes);

  void CreateInstanced(const RawDataView& vertexData,
                       const RawArrayView& indexData,
                       const ShaderAttributes& shaderAttributes,
                       const RawArrayView& instancesData,
                       const ShaderAttributes& instanceShaderAttributes);

  void CreateInstanced(const RawDataView& vertexData,
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

  void Configure(const VertexRenderData& buffer,
                 const ShaderAttributes& shaderAttributes);

  void RenderInstanced(const VertexRenderData& buffer) const;

  void RenderSingle(const VertexRenderData& buffer) const;

  friend class BufferPrivate;

  unsigned m_VAO = 0;

  std::unique_ptr<VertexRenderData> m_ownBuffer = nullptr;
  const VertexRenderData* m_referenceBuffer = nullptr;

  std::unique_ptr<InstancedDataBuffer> m_instancedBuffer = nullptr;
};

//---------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------