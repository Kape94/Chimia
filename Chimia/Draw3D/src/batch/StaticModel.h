#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"
#include "Model.h"
#include "ObjectTable.h"

#include "Core/DataBuffer.h"
#include "Rendering/InstancedBuffer.h"
#include "Rendering/ShaderAttribute.h"

#include <functional>
#include <initializer_list>
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

  unsigned AddInstance(const RawDataView& instanceData);
  unsigned AddInstance(const std::initializer_list<RawDataView>& instanceDatas);

  void DeleteInstance(unsigned instanceID);

  void Render();

private:
  void CreateGPUBuffers(const Model& model,
                        const size_t batchSize,
                        const size_t instanceBatchDataSize,
                        const Rendering::ShaderAttributes& vertexAttributes,
                        const Rendering::ShaderAttributes& instanceAttributes);

  bool HasSomethingToRender() const;

  bool CanRenderWithCurrentBuffers() const;

  void RebuildInputBuffer();

  void RenderByBatches();

  void HandleRenderingForBatchRange(const size_t rangeStart,
                                    const size_t rangeSize);

  void LoadBatchAndRender(const void* instancesData,
                          const unsigned instancesDataSize,
                          const unsigned nInstances);

  void RenderCurrentBuffers();

  std::function<void()> m_onRender;

  bool m_shouldRebuildBuffers = true;

  size_t m_instanceDataSize = 0;
  size_t m_instanceBatchDataSize = 0;
  DataBuffer m_instanceDataBuffer;
  ObjectTable<DataBuffer> m_instanceTable;
  std::vector<Rendering::InstancedBuffer> m_gpuBuffers;
};

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------