#pragma once

// ----------------------------------------------------------------------------

#include "Bits/Buffer/RawDataView.h"
#include "Draw3DNamespaceDefs.h"
#include "Model.h"
#include "ObjectTable.h"

#include "Bits/Buffer/RawBuffer.h"
#include "Rendering/InstancedBuffer.h"
#include "Rendering/ShaderAttribute.h"

#include <functional>
#include <vector>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

class StaticModel
{
public:
  void Create(const Model& model,
              const size_t batchSize,
              const Rendering::ShaderAttributes& vertexAttributes,
              const Rendering::ShaderAttributes& instanceAttributes,
              const std::function<void()>& onRender);

  unsigned AddInstance(const Bits::RawDataView& instanceData);
  void DeleteInstance(unsigned instanceID);

  void Render();

private:
  void RenderBatch(const void* instancesData,
                   const unsigned instancesDataSize,
                   const unsigned nInstances);

  std::function<void()> m_onRender;

  bool m_shouldRebuildBuffers = true;

  size_t m_instanceDataSize = 0;
  size_t m_instanceBatchDataSize = 0;
  Bits::RawBuffer m_instanceDataBuffer;
  ObjectTable<Bits::RawBuffer> m_instanceTable;
  std::vector<Rendering::InstancedBuffer> m_gpuBuffers;
};

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------