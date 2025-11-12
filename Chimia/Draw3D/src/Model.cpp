#include "Model.h"

#include "Core/Types.h"
#include "Rendering/ReusableIndexedVertexBufferObject.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

void
Model::Create(const MeshDataView& meshData)
{
  AllocateBufferDataOnGPU(meshData);
}

// ----------------------------------------------------------------------------

void
Model::Create(const std::vector<MeshDataView>& meshDatas)
{
  for (const MeshDataView& data : meshDatas) {
    AllocateBufferDataOnGPU(data);
  }
}

// ----------------------------------------------------------------------------

void
Model::AllocateBufferDataOnGPU(const MeshDataView& meshData)
{
  const RawDataView& vertex = meshData.VertexData();
  const RawArrayView& index = meshData.Indices();

  auto& insertedBuffer = m_gpuBufferDatas.emplace_back();
  insertedBuffer.Create(vertex, meshData.NVertices(), index);
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