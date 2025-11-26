#include "StaticModel.h"

#include "BatchUtils.h"
#include "Core/DataBuffer.h"
#include "Core/Types.h"
#include "Rendering/InstancedBuffer.h"
#include "Rendering/ReusableIndexedVertexBufferObject.h"
#include "Rendering/ShaderAttribute.h"

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
    instanceAttributes.ComputeTotalSizeOfAttributes();
  const size_t instanceBatchDataSize = batchSize * instanceDataSize;

  CreateGPUBuffers(model,
                   batchSize,
                   instanceBatchDataSize,
                   vertexAttributes,
                   instanceAttributes);

  m_instanceDataSize = instanceDataSize;
  m_instanceBatchDataSize = instanceBatchDataSize;
  m_instanceDataBuffer.Resize(instanceBatchDataSize);
}

// ----------------------------------------------------------------------------

void
StaticModel::CreateGPUBuffers(
  const Model& model,
  const size_t batchSize,
  const size_t instanceBatchDataSize,
  const Rendering::ShaderAttributes& vertexAttributes,
  const Rendering::ShaderAttributes& instanceAttributes)
{
  model.ForEachBuffer(
    [&](const Rendering::ReusableIndexedVertexBufferObject& buffer) {
      Rendering::InstancedBuffer& gpuBuffer = m_gpuBuffers.emplace_back();

      gpuBuffer.CreateInstanced(
        buffer,
        vertexAttributes,
        RawArrayView{ nullptr, batchSize, instanceBatchDataSize },
        instanceAttributes);
    });
}

// ----------------------------------------------------------------------------

unsigned
StaticModel::AddInstance(const RawDataView& instanceData)
{
  auto [instanceID, newInstance] = m_instanceTable.Insert();
  newInstance->Append(instanceData);

  m_shouldRebuildBuffers = true;

  return instanceID;
}

// ----------------------------------------------------------------------------

unsigned
StaticModel::AddInstance(
  const std::initializer_list<RawDataView>& instanceDatas)
{
  auto [instanceID, newInstance] = m_instanceTable.Insert();
  for (const RawDataView& data : instanceDatas) {
    newInstance->Append(data);
  }

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
  if (CanRenderWithCurrentBuffers()) {
    if (HasSomethingToRender()) {
      m_onRender();
      RenderCurrentBuffers();
    }
    return;
  }

  if (m_shouldRebuildBuffers) {
    RebuildInputBuffer();
  }

  RenderByBatches();
}

// ----------------------------------------------------------------------------

bool
StaticModel::CanRenderWithCurrentBuffers() const
{
  const bool needToReloadDataOnGPU = m_shouldRebuildBuffers;
  const bool needToRenderByBatch =
    m_instanceDataBuffer.GetSize() > m_instanceBatchDataSize;

  return !needToReloadDataOnGPU && !needToRenderByBatch;
}

// ----------------------------------------------------------------------------

bool
StaticModel::HasSomethingToRender() const
{
  return m_instanceDataBuffer.GetSize() > 0;
}

// ----------------------------------------------------------------------------

void
StaticModel::RebuildInputBuffer()
{
  m_instanceDataBuffer.Reset();
  m_instanceTable.ForEach([&](const DataBuffer& instanceData) {
    m_instanceDataBuffer.Append(instanceData.GetData(), instanceData.GetSize());
  });
  m_shouldRebuildBuffers = false;
}

// ----------------------------------------------------------------------------

void
StaticModel::RenderByBatches()
{
  if (!HasSomethingToRender()) {
    return;
  }

  m_onRender();

  BatchUtils::ForEachBatchRange(
    m_instanceDataBuffer.GetSize(),
    m_instanceBatchDataSize,
    [&](const size_t rangeStart, const size_t rangeSize) {
      HandleRenderingForBatchRange(rangeStart, rangeSize);
    });
}

// ----------------------------------------------------------------------------

void
StaticModel::HandleRenderingForBatchRange(const size_t rangeStart,
                                          const size_t rangeSize)
{
  const unsigned char* data = m_instanceDataBuffer.GetData();
  const unsigned char* offsetData = data + rangeStart;

  const unsigned nInstances = rangeSize / m_instanceDataSize;

  LoadBatchAndRender(offsetData, rangeSize, nInstances);
}

// ----------------------------------------------------------------------------

void
StaticModel::LoadBatchAndRender(const void* instancesData,
                                const unsigned instancesDataSize,
                                const unsigned nInstances)
{
  for (Rendering::InstancedBuffer& gpuBuffer : m_gpuBuffers) {
    gpuBuffer.LoadInstancedData(
      RawArrayView{ instancesData, nInstances, instancesDataSize });
    gpuBuffer.Render();
  }
}

// ----------------------------------------------------------------------------

void
StaticModel::RenderCurrentBuffers()
{
  for (Rendering::InstancedBuffer& gpuBuffer : m_gpuBuffers) {
    gpuBuffer.Render();
  }
}

// ----------------------------------------------------------------------------