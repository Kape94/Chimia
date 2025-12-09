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

  constexpr size_t N_VERTICES_IN_TRIANGLE = 3;
  m_inputDataSize =
    vertexAttributes.ComputeTotalSizeOfAttributes() * N_VERTICES_IN_TRIANGLE;

  const size_t batchSize = batchingSettings.initialBatchSize;
  const size_t bufferTotalSize = batchSize * m_inputDataSize;
  m_inputBuffer.Resize(bufferTotalSize);

  m_gpuBuffer.Create(RawDataView{ nullptr, bufferTotalSize }, vertexAttributes);

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
  const size_t batchSizeInBytes = m_inputBuffer.GetMaximumSize();

  BatchUtils::ForEachBatchRange(
    arraySizeInBytes,
    batchSizeInBytes,
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
  if (m_inputBuffer.GetAvailableSize() < m_inputDataSize) {
    m_onFlush();
    Flush();
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

  m_onFlush();

  m_gpuBuffer.Load(RawDataView{ m_inputBuffer.GetData(), frameInputSize });
  m_gpuBuffer.Render();

  m_inputBuffer.Reset();
}

// ----------------------------------------------------------------------------