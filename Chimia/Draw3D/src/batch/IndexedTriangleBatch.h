#pragma once

#include "Draw3DNamespaceDefs.h"

#include "Bits/Buffer/RawBuffer.h"
#include "Bits/Buffer/RawDataView.h"
#include "Rendering/IndexedBuffer.h"
#include "Rendering/ShaderAttribute.h"

#include <functional>

#include <glm/vec3.hpp>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

class IndexedTriangleBatch
{
public:
  void Create(const size_t vertexBatchSize,
              const size_t indexBatchSize,
              const Rendering::ShaderAttributes& vertexAttributes,
              const std::function<void(void)>& onFlush);

  void Draw(const Bits::RawArrayView& vertexDataView,
            const std::vector<unsigned>& indexData);

  void Flush();

private:
  size_t CalculateVertexDataSize(
    const Rendering::ShaderAttributes& vertexAttributes);

  std::vector<unsigned> RebaseIndicesAndAdjustBaseIndex(
    const std::vector<unsigned>& indexData);

  std::function<void(void)> m_onFlush;

  Bits::RawBuffer m_vertexInputBuffer;
  Bits::RawBuffer m_indexInputBuffer;

  unsigned m_baseIndex = 0;
  Rendering::IndexedBuffer m_gpuBuffer;
};

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------