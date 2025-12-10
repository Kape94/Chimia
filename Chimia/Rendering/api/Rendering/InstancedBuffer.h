#pragma once

#include "Core/ClassDefs.h"
#include "Core/Types.h"
#include "RenderingNamespaceDefs.h"

#include "IndexedBuffer.h"
#include "ReusableIndexedVertexBufferObject.h"
#include "ReusableVertexBufferObject.h"
#include "ShaderAttribute.h"

#include <variant>

//---------------------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

class InstancedBuffer
{
public:
  DEFAULT_CONSTUCTIBLE(InstancedBuffer)
  NON_COPYABLE(InstancedBuffer)

  InstancedBuffer(InstancedBuffer&& other) noexcept;
  InstancedBuffer& operator=(InstancedBuffer&& other) noexcept;

  ~InstancedBuffer();

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

  void Clear();

  void Render() const;

private:
  void LoadIndexDataInGPU(const unsigned* indexData,
                          const unsigned nIndexDataItems);

  void LoadInstancedDataInGPU(const void* instancedData,
                              const unsigned instanceDataSize,
                              const unsigned nInstances);

  void ClearBaseBuffer();

  void RenderWithRegularBaseBuffer(const Buffer& buffer) const;
  void RenderWithIndexedBaseBuffer(const IndexedBuffer& indexedBuffer) const;

  friend class BufferPrivate;

  std::variant<Buffer, IndexedBuffer> m_baseBuffer;
  unsigned m_instancedVBO = 0;
  unsigned m_nInstances = 0;
};

//---------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE
