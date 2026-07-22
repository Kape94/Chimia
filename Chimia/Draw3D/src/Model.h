#pragma once

// ----------------------------------------------------------------------------

#include "Core/ClassDefs.h"
#include "Draw3DNamespaceDefs.h"
#include "Types.h"

#include "Rendering/IndexData.h"
#include "Rendering/VertexData.h"

#include <functional>
#include <vector>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

class Model
{
public:
  DEFAULT_CONSTUCTIBLE(Model)
  NON_COPYABLE_NON_MOVABLE(Model)

  void Create(const MeshDataView& meshData, const eVertexLayout vertexLayout);

  void Create(const std::vector<MeshDataView>& meshDatas,
              const eVertexLayout vertexLayout);

  using BufferHandlerFn = std::function<void(const Rendering::VertexData&,
                                             const Rendering::IndexData&)>;
  void ForEachBuffer(const BufferHandlerFn& handleBuffer) const;

  eVertexLayout GetVertexLayout() const;

private:
  void AllocateBufferDataOnGPU(const MeshDataView& bufferData);

  eVertexLayout m_vertexLayout;
  std::vector<Rendering::VertexData> m_gpuVertexDatas;
  std::vector<Rendering::IndexData> m_gpuIndexDatas;
};

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------