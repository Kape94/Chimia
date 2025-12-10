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

  void DoFlushing();

  void HandleDynamicResizing();

  void Resize(size_t batchSize);

  size_t CurrentBatchSize() const;

  // Fixed attributes, defined only on creation
  std::function<void(void)> m_onFlush;
  BatchingSettings m_batchingSettings;
  Rendering::ShaderAttributes m_vertexAttributes;
  size_t m_triangleSizeInBytes = 0;

  size_t m_trianglesFlushedByDemand = 0;

  // These attributes are changed often as rendering requests came in
  DataBuffer m_inputBuffer;
  Rendering::Buffer m_gpuBuffer;
};

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------