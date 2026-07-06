#pragma once

#include "Core/ClassDefs.h"
#include "Core/Types.h"
#include "RenderingNamespaceDefs.h"

#include "IndexedRenderAction.h"
#include "ReusableIndexedVertexBufferObject.h"
#include "ReusableVertexBufferObject.h"
#include "ShaderAttribute.h"

#include <variant>

//---------------------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

class InstancedRenderAction
{
public:
  DEFAULT_CONSTUCTIBLE(InstancedRenderAction)
  NON_COPYABLE(InstancedRenderAction)

  InstancedRenderAction(InstancedRenderAction&& other) noexcept;
  InstancedRenderAction& operator=(InstancedRenderAction&& other) noexcept;

  ~InstancedRenderAction();

  void CreateInstanced(const ReusableVertexBufferObject& reusableVertexBuffer,
                       const ShaderAttributes& shaderAttributes,
                       const RawArrayView& instancesData,
                       const ShaderAttributes& instanceShaderAttributes);

  void CreateInstanced(
    const ReusableIndexedVertexBufferObject& reusableVertexBuffer,
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

  void LoadInstancedData(const RawArrayView& instancesData);

  void RecreateInstancedBuffer(
    const RawArrayView& instancesData,
    const ShaderAttributes& instanceShaderAttributes);

  void Clear();

  void Render() const;

private:
  void LoadIndexDataInGPU(const unsigned* indexData,
                          const unsigned nIndexDataItems);

  void CreateInstancedBuffer(const RawArrayView& instancesData,
                             const ShaderAttributes& instanceShaderAttributes);

  void LoadInstancedDataInGPU(const void* instancedData,
                              const unsigned instanceDataSize,
                              const unsigned nInstances);

  void ClearBaseBuffer();
  void ClearInstancesBuffer(unsigned& instanceVBO);

  unsigned GetBaseVAO();

  void RenderWithRegularBaseBuffer(const RenderAction& buffer) const;
  void RenderWithIndexedBaseBuffer(
    const IndexedRenderAction& indexedBuffer) const;

  friend class BufferPrivate;

  std::variant<RenderAction, IndexedRenderAction> m_baseBuffer;
  unsigned m_instancedVBO = 0;
  unsigned m_nInstances = 0;
};

//---------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE
