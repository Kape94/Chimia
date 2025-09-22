#pragma once

#include <vector>

#include "RenderLibNamespaceDefs.h"

#include "IndexedBuffer.h"
#include "ShaderAttribute.h"

//---------------------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

class InstancedBuffer
{
public:
  InstancedBuffer() = default;

  InstancedBuffer(InstancedBuffer&& other) noexcept;

  InstancedBuffer& operator=(InstancedBuffer&& other) noexcept;

  ~InstancedBuffer();

  void CreateInstanced(const float* vertexData,
                       const unsigned nVertexDataItems,
                       const unsigned* indexData,
                       const unsigned nIndexDataItems,
                       const ShaderAttributes& shaderAttributes,
                       const void* instancedData,
                       const unsigned instancedDataSize,
                       const unsigned nInstances,
                       const ShaderAttributes& instanceShaderAttributes);

  void Clear();

  void Render() const;

private:
  void LoadIndexDataInGPU(const unsigned* indexData,
                          const unsigned nIndexDataItems);

  void LoadInstancedDataInGPU(const void* instancedData,
                              const unsigned instanceDataSize,
                              const unsigned nInstances);

  friend class BufferPrivate;

  IndexedBuffer m_baseBuffer;
  unsigned m_instancedVBO = 0;
  unsigned m_nInstances = 0;
};

//---------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE
