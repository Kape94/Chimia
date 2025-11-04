#include "Model.h"

#include "BufferData.h"
#include "Rendering/ReusableIndexedVertexBufferObject.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

void
Model::Create(const BufferData& bufferData)
{
  AllocateBufferDataOnGPU(bufferData);
}

// ----------------------------------------------------------------------------

void
Model::Create(const std::vector<BufferData>& bufferDatas)
{
  for (const BufferData& data : bufferDatas) {
    AllocateBufferDataOnGPU(data);
  }
}

// ----------------------------------------------------------------------------

void
Model::AllocateBufferDataOnGPU(const BufferData& bufferData)
{
  const std::vector<float>& vertex = bufferData.VertexData();
  const unsigned nVertices = vertex.size();
  const unsigned vertexDataSize = vertex.size() * sizeof(float);

  const std::vector<unsigned>& index = bufferData.Indices();
  const unsigned nIndices = index.size();

  auto& insertedBuffer = m_gpuBufferDatas.emplace_back();
  insertedBuffer.Create(
    vertex.data(), vertexDataSize, nVertices, index.data(), nIndices);
}

// ----------------------------------------------------------------------------

void
Model::ForEachBuffer(const BufferHandlerFn& handleBuffer) const
{
  for (const auto& buffer : m_gpuBufferDatas) {
    handleBuffer(buffer);
  }
}

// ----------------------------------------------------------------------------