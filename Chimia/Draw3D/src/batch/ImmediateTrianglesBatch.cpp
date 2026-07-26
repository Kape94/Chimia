#include "ImmediateTrianglesBatch.h"

#include "BatchUtils.h"
#include "Core/Types.h"
#include "Rendering/ShaderAttribute.h"
#include "Rendering/VertexData.h"
#include "eImmediateFlushingPolicy.h"

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
ImmediateTrianglesBatch::Create(
  const BatchingSettings& batchingSettings,
  const Rendering::ShaderAttributes& vertexAttributes,
  const std::function<void(void)>& onFlush)
{
  m_batchingSettings = batchingSettings;
  m_vertexAttributes = vertexAttributes;

  constexpr size_t N_VERTICES_IN_TRIANGLE = 3;
  const size_t triangleSizeInBytes =
    vertexAttributes.ComputeTotalSizeOfAttributes() * N_VERTICES_IN_TRIANGLE;

  const size_t batchSize = batchingSettings.initialBatchSize;
  const size_t batchSizeInBytes = batchSize * triangleSizeInBytes;
  m_inputBuffer.Resize(batchSizeInBytes);

  m_gpuComponent.data = Rendering::VertexData::New();

  RawDataView rawData{ nullptr, batchSizeInBytes };
  m_gpuComponent.data->Create(
    rawData, CalculateNumberOfVertices(rawData, vertexAttributes));

  m_gpuComponent.action.Create(m_gpuComponent.data, vertexAttributes);

  m_currentGpuBufferSizeInBytes = batchSizeInBytes;

  m_triangleSizeInBytes = triangleSizeInBytes;
  m_onFlush = onFlush;
}

// ----------------------------------------------------------------------------

void
ImmediateTrianglesBatch::Draw(
  const std::initializer_list<RawDataView>& vertexDatas)
{
  for (const auto& vertexDataView : vertexDatas) {
    m_inputBuffer.Append(vertexDataView);
  }
}

// ----------------------------------------------------------------------------

void
ImmediateTrianglesBatch::Draw(const RawArrayView& vertexDataArray)
{
  m_inputBuffer.Append(vertexDataArray.AsDataView());
}

// ----------------------------------------------------------------------------

void
ImmediateTrianglesBatch::Flush(const eImmediateFlusingPolicy flushingPolicy)
{
  const size_t frameInputSize = m_inputBuffer.GetSize();
  if (frameInputSize == 0) {
    return;
  }

  if (!BatchUtils::ShouldKeepInput(flushingPolicy)) {
    HandleDynamicResizing();
  }

  DoFlushing(flushingPolicy);
}

// ----------------------------------------------------------------------------

void
ImmediateTrianglesBatch::DoFlushing(
  const eImmediateFlusingPolicy flushingPolicy)
{
  m_onFlush();

  const size_t inputSizeInBytes = m_inputBuffer.GetSize();
  BatchUtils::RenderByBatches(inputSizeInBytes,
                              m_currentGpuBufferSizeInBytes,
                              m_inputBuffer,
                              m_gpuComponent);

  if (!BatchUtils::ShouldKeepInput(flushingPolicy)) {
    m_inputBuffer.Reset();
  }
}

// ----------------------------------------------------------------------------

void
ImmediateTrianglesBatch::HandleDynamicResizing()
{
  const size_t maximumAllowed = m_batchingSettings.maximumBatchSize;
  const size_t currentGPUBatchSize = CurrentGPUBatchSize();

  const size_t inputSizeInBytes = m_inputBuffer.GetSize();
  const size_t inputAmmount = inputSizeInBytes / m_triangleSizeInBytes;

  if (currentGPUBatchSize < maximumAllowed &&
      currentGPUBatchSize < inputAmmount) {
    const size_t newBatchSize = std::min(inputAmmount, maximumAllowed);
    Resize(newBatchSize);
  }
}

// ----------------------------------------------------------------------------

void
ImmediateTrianglesBatch::Resize(size_t batchSize)
{
  const size_t newBatchSizeInBytes = batchSize * m_triangleSizeInBytes;

  RawDataView rawData{ nullptr, newBatchSizeInBytes };
  m_gpuComponent.data->Create(
    rawData, CalculateNumberOfVertices(rawData, m_vertexAttributes));

  m_gpuComponent.action.Create(m_gpuComponent.data, m_vertexAttributes);

  m_currentGpuBufferSizeInBytes = newBatchSizeInBytes;
}

// ----------------------------------------------------------------------------

size_t
ImmediateTrianglesBatch::CurrentGPUBatchSize() const
{
  return m_currentGpuBufferSizeInBytes / m_triangleSizeInBytes;
}

// ----------------------------------------------------------------------------