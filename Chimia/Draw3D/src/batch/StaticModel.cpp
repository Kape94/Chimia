#include "StaticModel.h"
#include "Bits/Buffer/RawBuffer.h"
#include "Rendering/InstancedBuffer.h"
#include "Rendering/ReusableIndexedVertexBufferObject.h"
#include "Rendering/ShaderAttribute.h"
#include <numeric>

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

void
StaticModel::Create(const Model& model,
                    const size_t batchSize,
                    const Rendering::ShaderAttributes& vertexAttributes,
                    const Rendering::ShaderAttributes& instanceAttributes,
                    const std::function<void()>& onRender)
{
  m_onRender = onRender;
  const size_t instanceDataSize =
    std::accumulate(instanceAttributes.begin(),
                    instanceAttributes.end(),
                    0,
                    [](size_t current, const Rendering::ShaderAttribute& attr) {
                      return current + attr.DataSizeInBytes();
                    });
  const size_t instanceBatchDataSize = batchSize * instanceDataSize;
  model.ForEachBuffer(
    [&](const Rendering::ReusableIndexedVertexBufferObject& buffer) {
      Rendering::InstancedBuffer& gpuBuffer = m_gpuBuffers.emplace_back();

      gpuBuffer.CreateInstanced(buffer,
                                vertexAttributes,
                                nullptr,
                                instanceBatchDataSize,
                                batchSize,
                                instanceAttributes);
    });

  m_instanceDataSize = instanceDataSize;
  m_instanceBatchDataSize = instanceBatchDataSize;
  m_instanceDataBuffer.Resize(instanceBatchDataSize);
}

// ----------------------------------------------------------------------------

unsigned
StaticModel::AddInstance(const Bits::RawDataView& instanceData)
{
  auto [instanceID, newInstance] = m_instanceTable.Insert();
  newInstance->Append(instanceData);

  m_shouldRebuildBuffers = true;

  return instanceID;
}

// ----------------------------------------------------------------------------

void
StaticModel::DeleteInstance(unsigned instanceID)
{
  m_instanceTable.Delete(instanceID);
  m_shouldRebuildBuffers = true;
}

// ----------------------------------------------------------------------------

void
StaticModel::Render()
{
  if (m_shouldRebuildBuffers) {
    m_instanceDataBuffer.Reset();
    m_instanceTable.ForEach([&](Bits::RawBuffer& instanceData) {
      m_instanceDataBuffer.Append(instanceData.GetData(),
                                  instanceData.GetSize());
    });
  }

  m_onRender();

  if (m_instanceDataBuffer.GetSize() > m_instanceBatchDataSize) {

    const size_t maxOffset =
      m_instanceDataBuffer.GetSize() / m_instanceBatchDataSize;
    for (size_t i = 0; i <= maxOffset; ++i) {
      const size_t offset = m_instanceBatchDataSize * i;
      const size_t nextOffset = m_instanceBatchDataSize * (i + 1);
      const size_t batchEnd =
        std::min(nextOffset, m_instanceDataBuffer.GetSize());
      const size_t batchSize = batchEnd - offset;

      const unsigned char* data = m_instanceDataBuffer.GetData();
      const unsigned char* offsetData = data + offset;

      const unsigned nInstances = batchSize / m_instanceDataSize;
      RenderBatch(offsetData, batchSize, nInstances);
    }
  } else {
    const unsigned nInstances =
      m_instanceDataBuffer.GetSize() / m_instanceDataSize;
    RenderBatch(m_instanceDataBuffer.GetData(),
                m_instanceDataBuffer.GetSize(),
                nInstances);
  }
}

// ----------------------------------------------------------------------------

void
StaticModel::RenderBatch(const void* instancesData,
                         const unsigned instancesDataSize,
                         const unsigned nInstances)
{
  for (Rendering::InstancedBuffer& gpuBuffer : m_gpuBuffers) {
    gpuBuffer.LoadInstancedData(instancesData, instancesDataSize, nInstances);
  }
  for (Rendering::InstancedBuffer& gpuBuffer : m_gpuBuffers) {
    gpuBuffer.Render();
  }
}

// ----------------------------------------------------------------------------