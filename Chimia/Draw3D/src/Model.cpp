#include "Model.h"

#include "Core/Types.h"
#include "Rendering/IndexData.h"
#include "Rendering/VertexData.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

void
Model::Create(const MeshDataView& meshData, const eVertexLayout vertexLayout)
{
  AllocateBufferDataOnGPU(meshData);
  m_vertexLayout = vertexLayout;
}

// ----------------------------------------------------------------------------

void
Model::Create(const std::vector<MeshDataView>& meshDatas,
              const eVertexLayout vertexLayout)
{
  for (const MeshDataView& data : meshDatas) {
    AllocateBufferDataOnGPU(data);
  }
  m_vertexLayout = vertexLayout;
}

// ----------------------------------------------------------------------------

void
Model::AllocateBufferDataOnGPU(const MeshDataView& meshData)
{
  const RawDataView& vertex = meshData.VertexData();
  const RawArrayView& index = meshData.Indices();

  auto& insertedVertexData =
    m_gpuVertexDatas.emplace_back(Rendering::VertexData::New());
  insertedVertexData->Create(vertex, meshData.NVertices());

  auto& insertedIndexData =
    m_gpuIndexDatas.emplace_back(Rendering::IndexData::New());
  insertedIndexData->Create(index);
}

// ----------------------------------------------------------------------------

void
Model::ForEachBuffer(const BufferHandlerFn& handleBuffer) const
{
  const size_t nDatas = m_gpuVertexDatas.size();
  for (size_t i = 0; i < nDatas; ++i) {
    const Rendering::VertexDataInstance& vData = m_gpuVertexDatas[i];
    const Rendering::IndexDataInstance& iData = m_gpuIndexDatas[i];

    handleBuffer(vData, iData);
  }
}

// ----------------------------------------------------------------------------

eVertexLayout
Model::GetVertexLayout() const
{
  return m_vertexLayout;
}

// ----------------------------------------------------------------------------