#include "TriangleBatch.h"
#include "Bits/Buffer/RawDataView.h"
#include "Rendering/ShaderAttribute.h"
#include <numeric>

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
    CalculateVertexDataSize(vertexAttributes) * N_VERTICES_IN_TRIANGLE;

  const size_t bufferTotalSize = batchSize * m_inputDataSize;
  m_inputBuffer.Resize(bufferTotalSize);

  m_gpuBuffer.Create(nullptr, bufferTotalSize, vertexAttributes);

  m_onFlush = onFlush;
}

// ----------------------------------------------------------------------------

size_t
TriangleBatch::CalculateVertexDataSize(
  const Rendering::ShaderAttributes& vertexAttributes)
{
  return std::accumulate(
    vertexAttributes.begin(),
    vertexAttributes.end(),
    0,
    [](size_t current, const Rendering::ShaderAttribute& attr) {
      return current + attr.DataSizeInBytes();
    });
}

// ----------------------------------------------------------------------------

void
TriangleBatch::Draw(const std::initializer_list<Bits::RawDataView>& vertexDatas)
{
  if (m_inputBuffer.GetAvailableSize() < m_inputDataSize) {
    m_onFlush();
    Flush();
  }

  for (const auto& vertexDataView : vertexDatas) {
    m_inputBuffer.Append(vertexDataView);
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

  m_gpuBuffer.Load(m_inputBuffer.GetData(), frameInputSize);
  m_gpuBuffer.Render();

  m_inputBuffer.Reset();
}

// ----------------------------------------------------------------------------