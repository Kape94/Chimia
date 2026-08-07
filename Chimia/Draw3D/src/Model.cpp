#include "Model.h"

#include "Core/Types.h"
#include "Rendering/DataLayout.h"
#include "Rendering/IndexData.h"
#include "Rendering/VertexData.h"
#include "Types.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace {
Chimia::Rendering::DataLayout
DataLayoutForVertex(const eVertexLayout vertexLayout)
{
  switch (vertexLayout) {
    case eVertexLayout::POSITION3_COLOR4:
      return { { "position", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
               { "color", Chimia::Rendering::eDataType::VECTOR_4_FLOAT } };
    case eVertexLayout::POSITION3_NORMAL3:
      return { { "position", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
               { "normal", Chimia::Rendering::eDataType::VECTOR_3_FLOAT } };
    case eVertexLayout::POSITION3_TEXCOORD2:
      return { { "position", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
               { "texCoord", Chimia::Rendering::eDataType::VECTOR_2_FLOAT } };
    case eVertexLayout::POSITION3_COLOR4_NORMAL3:
      return { { "position", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
               { "color", Chimia::Rendering::eDataType::VECTOR_4_FLOAT },
               { "normal", Chimia::Rendering::eDataType::VECTOR_3_FLOAT } };
    case eVertexLayout::POSITION3_COLOR4_TEXCOORD2:
      return { { "position", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
               { "color", Chimia::Rendering::eDataType::VECTOR_4_FLOAT },
               { "texCoord", Chimia::Rendering::eDataType::VECTOR_2_FLOAT } };
    case eVertexLayout::POSITION3_NORMAL3_TEXCOORD2:
      return { { "position", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
               { "normal", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
               { "texCoord", Chimia::Rendering::eDataType::VECTOR_2_FLOAT } };
    case eVertexLayout::POSITION3_COLOR4_NORMAL3_TEXCOORD2:
      return { { "position", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
               { "color", Chimia::Rendering::eDataType::VECTOR_4_FLOAT },
               { "normal", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
               { "texCoord", Chimia::Rendering::eDataType::VECTOR_2_FLOAT } };
    case eVertexLayout::UNDEFINED:
    default:
      return {};
  }
}
}

// ----------------------------------------------------------------------------

void
Model::Create(const MeshDataView& meshData, const eVertexLayout vertexLayout)
{
  AllocateBufferDataOnGPU(meshData, vertexLayout);
  m_vertexLayout = vertexLayout;
}

// ----------------------------------------------------------------------------

void
Model::Create(const std::vector<MeshDataView>& meshDatas,
              const eVertexLayout vertexLayout)
{
  for (const MeshDataView& data : meshDatas) {
    AllocateBufferDataOnGPU(data, vertexLayout);
  }
  m_vertexLayout = vertexLayout;
}

// ----------------------------------------------------------------------------

void
Model::AllocateBufferDataOnGPU(const MeshDataView& meshData,
                               const eVertexLayout vertexLayout)
{
  const RawDataView& vertex = meshData.VertexData();
  const RawArrayView& index = meshData.Indices();

  auto& insertedVertexData = m_gpuVertexDatas.emplace_back(
    Rendering::VertexData::Create(vertex, DataLayoutForVertex(vertexLayout)));

  auto& insertedIndexData =
    m_gpuIndexDatas.emplace_back(Rendering::IndexData::Create(index));
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