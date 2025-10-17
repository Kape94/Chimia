#include "TriangleBatch.h"
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

  m_vertexDataSize = CalculateVertexDataSize(vertexAttributes);
  const size_t bufferTotalSize = batchSize * m_vertexDataSize;

  m_inputBuffer.Resize(bufferTotalSize);

  const size_t nFloats = bufferTotalSize / sizeof(float);
  m_gpuBuffer.Create(nullptr, nFloats, vertexAttributes);

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
TriangleBatch::Draw(
  const std::initializer_list<Bits::RawDataView>& additionalVertexDatas)
{
  if (m_inputBuffer.GetAvailableSize() < m_vertexDataSize) {
    m_onFlush();
    Flush();
  }

  for (const auto& vertexDataView : additionalVertexDatas) {
    m_inputBuffer.Append(vertexDataView);
  }
}

// ----------------------------------------------------------------------------

void
TriangleBatch::Flush()
{
  const size_t inputSize = m_inputBuffer.GetSize();
  if (inputSize == 0) {
    return;
  }

  m_onFlush();

  const float* inputData = reinterpret_cast<float*>(m_inputBuffer.GetData());
  const unsigned nFloatItems = inputSize / sizeof(float);

  m_gpuBuffer.Load(inputData, nFloatItems);
  m_gpuBuffer.Render();

  m_inputBuffer.Reset();
}

// ----------------------------------------------------------------------------