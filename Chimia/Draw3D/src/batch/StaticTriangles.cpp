#include "StaticTriangles.h"

#include "BatchUtils.h"
#include "Core/DataBuffer.h"
#include "Core/Types.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

void
StaticTriangles::Create(const BatchingSettings& batchingSettings,
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
StaticTriangles::AddStaticMesh(const RawDataView& vertexData)
{
  auto [meshID, triangleData] = m_staticTrianglesTable.Insert();
  triangleData->Append(vertexData);

  m_shouldRebuildBuffers = true;

  return meshID;
}

// ----------------------------------------------------------------------------

void
StaticTriangles::DeleteStaticMesh(const unsigned meshID)
{
  m_staticTrianglesTable.Delete(meshID);
  m_shouldRebuildBuffers = true;
}

// ----------------------------------------------------------------------------

void
StaticTriangles::Render()
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
StaticTriangles::CanRenderWithCurrentBuffer() const
{
  const size_t trianglesDataInBytes = m_inputBuffer.GetSize();

  const bool needToReloadDataOnGPU = m_shouldRebuildBuffers;
  const bool needToRenderByBatches =
    trianglesDataInBytes > CurrentGPUBatchSizeInBytes();

  return !needToReloadDataOnGPU && !needToRenderByBatches;
}

// ----------------------------------------------------------------------------

bool
StaticTriangles::HasSomethingToRender() const
{
  return m_inputBuffer.GetSize() > 0;
}

// ----------------------------------------------------------------------------

void
StaticTriangles::RebuildTrianglesBuffer()
{
  m_inputBuffer.Reset();
  m_staticTrianglesTable.ForEach([&](const DataBuffer& triangleData) {
    m_inputBuffer.Append(triangleData.GetData(), triangleData.GetSize());
  });
  m_shouldRebuildBuffers = false;
}

// ----------------------------------------------------------------------------

void
StaticTriangles::HandleDynamicResizing()
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
StaticTriangles::ResizeGPUBatch(const size_t batchSize)
{
  const size_t batchSizeInBytes = batchSize * m_triangleSizeInBytes;
  m_gpuBuffer.Clear();
  m_gpuBuffer.Create(RawDataView{ nullptr, batchSizeInBytes },
                     m_vertexAttributes);

  m_currentGPUBatchSize = batchSize;
}

// ----------------------------------------------------------------------------

void
StaticTriangles::RenderByBatches()
{
  const size_t inputSize = m_inputBuffer.GetSize();
  if (inputSize == 0) {
    return;
  }

  BatchUtils::ForEachBatchRange(
    inputSize,
    CurrentGPUBatchSizeInBytes(),
    [&](const size_t rangeStart, const size_t rangeSize) {
      HandleRenderingForBatchRange(rangeStart, rangeSize);
    });
}

// ----------------------------------------------------------------------------

void
StaticTriangles::HandleRenderingForBatchRange(const size_t rangeStart,
                                              const size_t rangeSize)
{
  const unsigned char* data = m_inputBuffer.GetData();
  const unsigned char* batchData = data + rangeStart;

  m_gpuBuffer.Load(RawDataView{ batchData, rangeSize });
  m_gpuBuffer.Render();
}

// ----------------------------------------------------------------------------

size_t
StaticTriangles::CurrentGPUBatchSizeInBytes() const
{
  return m_currentGPUBatchSize * m_triangleSizeInBytes;
}

// ----------------------------------------------------------------------------