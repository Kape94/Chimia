#pragma once

#include "Draw3DNamespaceDefs.h"

#include "Core/DataBuffer.h"
#include "Rendering/Buffer.h"
#include "Rendering/ShaderAttribute.h"
#include "Types.h"

#include <functional>
#include <initializer_list>

#include <glm/vec3.hpp>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

class TriangleBatch
{
public:
  void Create(const BatchingSettings& batchingSettings,
              const Rendering::ShaderAttributes& vertexAttributes,
              const std::function<void(void)>& onFlush);

  void Draw(const std::initializer_list<RawDataView>& vertexDatas);
  void Draw(const RawArrayView& vertexDataArray);

  void Flush();

private:
  void HandleFlushByDemand();

  std::function<void(void)> m_onFlush;
  BatchingSettings m_batchingSettings;

  size_t m_inputDataSize = 0;
  DataBuffer m_inputBuffer;
  Rendering::Buffer m_gpuBuffer;
};

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------