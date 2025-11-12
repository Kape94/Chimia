#include "TriangleBatch.h"

#include "Core/Types.h"
#include "Rendering/ShaderAttribute.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

void
TriangleBatch::Create(const size_t batchSize,
                      const Rendering::ShaderAttributes& vertexAttributes,
                      const std::function<void(void)>& onFlush)
{
  constexpr size_t N_VERTICES_IN_TRIANGLE = 3;
  m_inputDataSize =
    vertexAttributes.ComputeTotalSizeOfAttributes() * N_VERTICES_IN_TRIANGLE;

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
  HandleFlushByDemand();
  m_inputBuffer.Append(vertexDataArray.AsDataView());
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