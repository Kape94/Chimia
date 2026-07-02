#include "RetainedTrianglesBatch.h"

#include "BatchUtils.h"
#include "Core/DataBuffer.h"
#include "Core/Types.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

void
RetainedTrianglesBatch::Create(
  const BatchingSettings& batchingSettings,
  const Rendering::ShaderAttributes& shaderAttributes)
{
  m_batchingSettings = batchingSettings;
  m_vertexAttributes = shaderAttributes;

  const size_t batchSize = batchingSettings.initialBatchSize;
  const size_t sizePerVertex = shaderAttributes.ComputeTotalSizeOfAttributes();
  constexpr size_t nVerticesPerTriangle = 3;
  const size_t triangleSizeInBytes = sizePerVertex * nVerticesPerTriangle;

  const size_t batchSizeInBytes = batchSize * triangleSizeInBytes;

  m_gpuBuffer.Create(RawDataView{ nullptr, batchSizeInBytes },
                     shaderAttributes);
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
      m_gpuBuffer.Render();
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
  const size_t batchSizeInBytes = batchSize * m_triangleSizeInBytes;
  m_gpuBuffer.Clear();
  m_gpuBuffer.Create(RawDataView{ nullptr, batchSizeInBytes },
                     m_vertexAttributes);

  m_currentGPUBatchSize = batchSize;
}

// ----------------------------------------------------------------------------

void
RetainedTrianglesBatch::RenderByBatches()
{
  const size_t inputSizeInBytes = m_inputBuffer.GetSize();
  if (inputSizeInBytes == 0) {
    return;
  }

  BatchUtils::RenderByBatches(
    inputSizeInBytes, CurrentGPUBatchSizeInBytes(), m_inputBuffer, m_gpuBuffer);
}

// ----------------------------------------------------------------------------

size_t
RetainedTrianglesBatch::CurrentGPUBatchSizeInBytes() const
{
  return m_currentGPUBatchSize * m_triangleSizeInBytes;
}

// ----------------------------------------------------------------------------