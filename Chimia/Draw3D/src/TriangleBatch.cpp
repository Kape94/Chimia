#include "TriangleBatch.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

void
TriangleBatch::Create(const size_t batchSize,
                      const Rendering::ShaderAttributes& vertexAttributes,
                      const std::function<void(void)>& onFlush)
{

  // TODO: use shader attrs to get total data size
  const size_t vertexSizePerTriangle = 6 * sizeof(glm::vec3);
  const size_t bufferTotalSize = batchSize * vertexSizePerTriangle;

  m_inputBuffer.Resize(bufferTotalSize);

  const size_t nFloats = bufferTotalSize / sizeof(float);
  m_gpuBuffer.Create(nullptr, nFloats, vertexAttributes);

  m_onFlush = onFlush;
}

// ----------------------------------------------------------------------------

void
TriangleBatch::Draw(
  const std::initializer_list<Bits::RawDataView>& additionalVertexDatas)
{
  // TODO: adjust size here...
  constexpr size_t VEC3_SIZE = sizeof(glm::vec3);
  constexpr size_t INCOMING_SIZE = 6 * VEC3_SIZE;

  if (m_inputBuffer.GetAvailableSize() < INCOMING_SIZE) {
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