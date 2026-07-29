#include "RetainedTrianglesBatch.h"

#include "BatchUtils.h"
#include "Core/DataBuffer.h"
#include "Core/Types.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
// TEST - will be removed soon
// ----------------------------------------------------------------------------

namespace {
unsigned
ComputeTotalSizeOfAttributes(const Chimia::Rendering::ShaderAttributes& attrs)
{
  unsigned totalSize = 0;
  for (const Chimia::Rendering::ShaderAttribute& attr : attrs) {
    totalSize += attr.DataSizeInBytes();
  }
  return totalSize;
}

size_t
CalculateNumberOfVertices(
  const Chimia::RawDataView& vertexData,
  const Chimia::Rendering::ShaderAttributes& shaderAttributes)
{
  const size_t vertexDataSize = vertexData.size;
  const size_t sizePerVertex = ComputeTotalSizeOfAttributes(shaderAttributes);
  return vertexDataSize / sizePerVertex;
}
}

// ----------------------------------------------------------------------------
// TEST - will be removed soon
// ----------------------------------------------------------------------------

void
RetainedTrianglesBatch::Create(
  const BatchingSettings& batchingSettings,
  const Rendering::DataLayout& vertexDataLayout,
  const Rendering::ShaderAttributes& shaderAttributes)
{
  m_batchingSettings = batchingSettings;
  m_vertexAttributes = shaderAttributes;

  const size_t batchSize = batchingSettings.initialBatchSize;
  const size_t sizePerVertex = shaderAttributes.ComputeTotalSizeOfAttributes();
  constexpr size_t nVerticesPerTriangle = 3;
  const size_t triangleSizeInBytes = sizePerVertex * nVerticesPerTriangle;

  const size_t batchSizeInBytes = batchSize * triangleSizeInBytes;

  m_gpuComponent.data = Rendering::VertexData::New();

  RawDataView rawData{ nullptr, batchSizeInBytes };
  m_gpuComponent.data->Create(rawData, vertexDataLayout);

  m_gpuComponent.action.Create(m_gpuComponent.data, shaderAttributes);

  m_inputBuffer.Resize(batchSizeInBytes);

  m_currentGPUBatchSize = batchSize;
  m_triangleSizeInBytes = triangleSizeInBytes;

  m_shouldRebuildBuffers = false;
}

// ----------------------------------------------------------------------------

unsigned
RetainedTrianglesBatch::AddStaticMesh(const RawDataView& vertexData)
{
  auto [meshID, triangleData] = m_staticTrianglesTable.Insert();
  triangleData->Append(vertexData);

  m_shouldRebuildBuffers = true;

  return meshID;
}

// ----------------------------------------------------------------------------

void
RetainedTrianglesBatch::DeleteStaticMesh(const unsigned meshID)
{
  m_staticTrianglesTable.Delete(meshID);
  m_shouldRebuildBuffers = true;
}

// ----------------------------------------------------------------------------

void
RetainedTrianglesBatch::Render()
{
  if (CanRenderWithCurrentBuffer()) {
    if (HasSomethingToRender()) {
      m_gpuComponent.action.Render();
    }
    return;
  }

  if (m_shouldRebuildBuffers) {
    RebuildTrianglesBuffer();
    HandleDynamicResizing();
  }

  RenderByBatches();
}

// ----------------------------------------------------------------------------

bool
RetainedTrianglesBatch::CanRenderWithCurrentBuffer() const
{
  const size_t trianglesDataInBytes = m_inputBuffer.GetSize();

  const bool needToReloadDataOnGPU = m_shouldRebuildBuffers;
  const bool needToRenderByBatches =
    trianglesDataInBytes > CurrentGPUBatchSizeInBytes();

  return !needToReloadDataOnGPU && !needToRenderByBatches;
}

// ----------------------------------------------------------------------------

bool
RetainedTrianglesBatch::HasSomethingToRender() const
{
  return m_inputBuffer.GetSize() > 0;
}

// ----------------------------------------------------------------------------

void
RetainedTrianglesBatch::RebuildTrianglesBuffer()
{
  m_inputBuffer.Reset();
  m_staticTrianglesTable.ForEach([&](const DataBuffer& triangleData) {
    m_inputBuffer.Append(triangleData.GetData(), triangleData.GetSize());
  });
  m_shouldRebuildBuffers = false;
}

// ----------------------------------------------------------------------------

void
RetainedTrianglesBatch::HandleDynamicResizing()
{
  const size_t maximumAllowed = m_batchingSettings.maximumBatchSize;
  const size_t gpuBatchSize = m_currentGPUBatchSize;

  const size_t trianglesIncoming =
    m_inputBuffer.GetSize() / m_triangleSizeInBytes;
  if (gpuBatchSize < maximumAllowed && gpuBatchSize != trianglesIncoming) {
    const size_t desiredNewSize = trianglesIncoming;
    const size_t newBatchSize = std::min(desiredNewSize, maximumAllowed);

    ResizeGPUBatch(newBatchSize);
  }
}

// ----------------------------------------------------------------------------

void
RetainedTrianglesBatch::ResizeGPUBatch(const size_t batchSize)
{
  const size_t effectiveBatchSize = BatchUtils::EffectiveBatchSize(batchSize);
  const size_t batchSizeInBytes = effectiveBatchSize * m_triangleSizeInBytes;

  m_gpuComponent.data->Resize({ nullptr, batchSizeInBytes });

  m_currentGPUBatchSize = effectiveBatchSize;
}

// ----------------------------------------------------------------------------

void
RetainedTrianglesBatch::RenderByBatches()
{
  const size_t inputSizeInBytes = m_inputBuffer.GetSize();
  if (inputSizeInBytes == 0) {
    return;
  }

  BatchUtils::RenderByBatches(inputSizeInBytes,
                              CurrentGPUBatchSizeInBytes(),
                              m_inputBuffer,
                              m_gpuComponent);
}

// ----------------------------------------------------------------------------

size_t
RetainedTrianglesBatch::CurrentGPUBatchSizeInBytes() const
{
  return m_currentGPUBatchSize * m_triangleSizeInBytes;
}

// ----------------------------------------------------------------------------