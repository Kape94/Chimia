#include "IndexedTriangleBatch.h"

#include "Rendering/ShaderAttribute.h"
#include <algorithm>
#include <iterator>
#include <numeric>
#include <vector>

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

void
IndexedTriangleBatch::Create(
  const size_t vertexBatchSize,
  const size_t indexBatchSize,
  const Rendering::ShaderAttributes& vertexAttributes,
  const std::function<void(void)>& onFlush)
{
  const size_t vertexInputDataSize = CalculateVertexDataSize(vertexAttributes);
  const size_t vertexBufferTotalSize = vertexBatchSize * vertexInputDataSize;
  m_vertexInputBuffer.Resize(vertexBufferTotalSize);

  const size_t indexBufferTotalSize = indexBatchSize * sizeof(unsigned);
  m_indexInputBuffer.Resize(indexBufferTotalSize);

  m_gpuBuffer.Create(nullptr,
                     vertexBufferTotalSize,
                     nullptr,
                     indexBufferTotalSize,
                     vertexAttributes);
  m_baseIndex = 0;

  m_onFlush = onFlush;
}

// ----------------------------------------------------------------------------

size_t
IndexedTriangleBatch::CalculateVertexDataSize(
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
IndexedTriangleBatch::Draw(const Bits::RawArrayView& vertexDataView,
                           const std::vector<unsigned>& indexData)
{
  const size_t vertexIncomingSize = vertexDataView.TotalSize();
  const size_t indexIncomingSize = indexData.size() * sizeof(unsigned);
  if (m_vertexInputBuffer.GetAvailableSize() < vertexIncomingSize ||
      m_indexInputBuffer.GetAvailableSize() < indexIncomingSize) {
    m_onFlush();
    Flush();
  }

  const std::vector<unsigned> rebasedIndices =
    RebaseIndicesAndAdjustBaseIndex(indexData);

  m_vertexInputBuffer.Append(vertexDataView.array, vertexIncomingSize);
  m_indexInputBuffer.Append(rebasedIndices.data(), indexIncomingSize);
}

// ----------------------------------------------------------------------------

std::vector<unsigned>
IndexedTriangleBatch::RebaseIndicesAndAdjustBaseIndex(
  const std::vector<unsigned>& indexData)
{
  unsigned maxIndex = 0;
  std::vector<unsigned> rebasedIndices;
  std::transform(indexData.begin(),
                 indexData.end(),
                 std::back_inserter(rebasedIndices),
                 [&](unsigned i) {
                   if (i > maxIndex) {
                     maxIndex = i;
                   }
                   return m_baseIndex + i;
                 });

  m_baseIndex += maxIndex + 1;

  return rebasedIndices;
}

// ----------------------------------------------------------------------------

void
IndexedTriangleBatch::Flush()
{
  const size_t frameVertexInputSize = m_vertexInputBuffer.GetSize();
  const size_t frameIndexInputSize = m_indexInputBuffer.GetSize();
  if (frameVertexInputSize == 0 && frameIndexInputSize == 0) {
    return;
  }

  m_onFlush();

  m_gpuBuffer.LoadVertexData(m_vertexInputBuffer.GetData(),
                             frameVertexInputSize);
  m_gpuBuffer.LoadIndexData(
    reinterpret_cast<const unsigned*>(m_indexInputBuffer.GetData()),
    frameVertexInputSize);
  m_gpuBuffer.Render();

  m_baseIndex = 0;
  m_vertexInputBuffer.Reset();
  m_indexInputBuffer.Reset();
}

// ----------------------------------------------------------------------------