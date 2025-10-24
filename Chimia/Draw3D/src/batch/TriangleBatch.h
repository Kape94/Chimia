#pragma once

#include "Draw3DNamespaceDefs.h"

#include "Bits/Buffer/RawBuffer.h"
#include "Bits/Buffer/RawDataView.h"
#include "Rendering/Buffer.h"
#include "Rendering/ShaderAttribute.h"

#include <functional>
#include <initializer_list>

#include <glm/vec3.hpp>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

class TriangleBatch
{
public:
  void Create(const size_t batchSize,
              const Rendering::ShaderAttributes& vertexAttributes,
              const std::function<void(void)>& onFlush);

  void Draw(const std::initializer_list<Bits::RawDataView>& vertexDatas);
  void Draw(const Bits::RawArrayView& vertexDataArray);

  void Flush();

private:
  size_t CalculateVertexDataSize(
    const Rendering::ShaderAttributes& vertexAttributes);

  void HandleFlushByDemand();

  std::function<void(void)> m_onFlush;

  size_t m_inputDataSize = 0;
  Bits::RawBuffer m_inputBuffer;
  Rendering::Buffer m_gpuBuffer;
};

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------