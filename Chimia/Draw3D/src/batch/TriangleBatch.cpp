#include "TriangleBatch.h"

#include "BatchUtils.h"
#include "Core/Types.h"
#include "Rendering/ShaderAttribute.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

void
TriangleBatch::Create(const BatchingSettings& batchingSettings,
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

  m_gpuBuffer.Create(RawDataView{ nullptr, batchSizeInBytes },
                     vertexAttributes);

  m_triangleSizeInBytes = triangleSizeInBytes;
  m_onFlush = onFlush;
}

// ----------------------------------------------------------------------------

void
TriangleBatch::Draw(const std::initializer_list<RawDataView>& vertexDatas)
{
  HandleFlushByDemand();

  for (const auto& vertexDataView : vertexDatas) {
    m_inputBuffer.Append(vertexDataView);
  }
}

// ----------------------------------------------------------------------------

void
TriangleBatch::Draw(const RawArrayView& vertexDataArray)
{
  const size_t arraySizeInBytes = vertexDataArray.TotalSize();

  BatchUtils::ForEachBatchRange(
    arraySizeInBytes,
    m_triangleSizeInBytes,
    [&](const size_t rangeStart, const size_t rangeSize) {
      const unsigned char* data =
        reinterpret_cast<const unsigned char*>(vertexDataArray.array);
      const unsigned char* offsetData = data + rangeStart;

      Draw({ RawDataView(offsetData, rangeSize) });
    });
}

// ----------------------------------------------------------------------------

void
TriangleBatch::HandleFlushByDemand()
{
  if (m_inputBuffer.GetAvailableSize() < m_triangleSizeInBytes) {
    const size_t nTrianglesInBuffer =
      m_inputBuffer.GetSize() / m_triangleSizeInBytes;
    m_trianglesFlushedByDemand += nTrianglesInBuffer;
    DoFlushing();
  }
}

// ----------------------------------------------------------------------------

void
TriangleBatch::Flush()
{
  const size_t frameInputSize = m_inputBuffer.GetSize();
  if (frameInputSize == 0) {
    return;
  }

  DoFlushing();

  HandleDynamicResizing();
}

// ----------------------------------------------------------------------------

void
TriangleBatch::DoFlushing()
{
  m_onFlush();

  m_gpuBuffer.Load(
    RawDataView{ m_inputBuffer.GetData(), m_inputBuffer.GetSize() });
  m_gpuBuffer.Render();

  m_inputBuffer.Reset();
}

// ----------------------------------------------------------------------------

void
TriangleBatch::HandleDynamicResizing()
{
  const size_t maximumAllowed = m_batchingSettings.maximumBatchSize;
  const size_t currentBatchSize = CurrentBatchSize();

  if (m_trianglesFlushedByDemand > 0 && currentBatchSize < maximumAllowed) {
    const size_t desiredBatchSize =
      currentBatchSize + m_trianglesFlushedByDemand;
    const size_t newBatchSize = std::min(desiredBatchSize, maximumAllowed);

    Resize(newBatchSize);
  }

  m_trianglesFlushedByDemand = 0;
}

// ----------------------------------------------------------------------------

void
TriangleBatch::Resize(size_t batchSize)
{
  const size_t newBatchSizeInBytes = batchSize * m_triangleSizeInBytes;
  m_inputBuffer.Resize(newBatchSizeInBytes);

  m_gpuBuffer.Clear();
  m_gpuBuffer.Create({ nullptr, newBatchSizeInBytes }, m_vertexAttributes);
}

// ----------------------------------------------------------------------------

size_t
TriangleBatch::CurrentBatchSize() const
{
  return m_inputBuffer.GetMaximumSize() / m_triangleSizeInBytes;
}

// ----------------------------------------------------------------------------