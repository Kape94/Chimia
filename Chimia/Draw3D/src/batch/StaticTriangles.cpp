#include "StaticTriangles.h"

#include "BatchUtils.h"
#include "Bits/Buffer/RawBuffer.h"
#include "Core/Types.h"
#include "Draw3DPrivate.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

void
StaticTriangles::Create(const size_t batchSize,
                        const Rendering::ShaderAttributes& shaderAttributes)
{
  const size_t sizePerVertex = shaderAttributes.ComputeTotalSizeOfAttributes();
  constexpr size_t nVerticesPerTriangle = 3;
  const size_t triangleBatchSize =
    sizePerVertex * nVerticesPerTriangle * batchSize;

  m_gpuBuffer.Create(RawDataView{ nullptr, triangleBatchSize },
                     shaderAttributes);
  m_inputBuffer.Resize(triangleBatchSize);

  m_batchSize = triangleBatchSize;
  m_shouldRebuildBuffers = false;
}

// ----------------------------------------------------------------------------

unsigned
StaticTriangles::AddStaticMesh(const std::vector<float>& vertexData)
{
  auto [meshID, triangleData] = m_staticTrianglesTable.Insert();
  triangleData->Append(vertexData.data(), vertexData.size() * sizeof(float));

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
  }

  RenderByBatches();
}

// ----------------------------------------------------------------------------

bool
StaticTriangles::CanRenderWithCurrentBuffer() const
{
  const size_t trianglesData = m_inputBuffer.GetSize();

  const bool needToReloadDataOnGPU = m_shouldRebuildBuffers;
  const bool needToRenderByBatches = trianglesData > m_batchSize;

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
  m_staticTrianglesTable.ForEach([&](const Bits::RawBuffer& triangleData) {
    m_inputBuffer.Append(triangleData.GetData(), triangleData.GetSize());
  });
  m_shouldRebuildBuffers = false;
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
    m_batchSize,
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