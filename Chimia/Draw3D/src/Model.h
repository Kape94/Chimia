#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"
#include "Types.h"

#include "Rendering/ReusableIndexedVertexBufferObject.h"

#include <functional>
#include <vector>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

class Model
{
public:
  void Create(const MeshDataView& meshData, const eVertexLayout vertexLayout);

  void Create(const std::vector<MeshDataView>& meshDatas,
              const eVertexLayout vertexLayout);

  using BufferHandlerFn =
    std::function<void(const Rendering::ReusableIndexedVertexBufferObject&)>;
  void ForEachBuffer(const BufferHandlerFn& handleBuffer) const;

  eVertexLayout GetVertexLayout() const;

private:
  void AllocateBufferDataOnGPU(const MeshDataView& bufferData);

  eVertexLayout m_vertexLayout;
  std::vector<Rendering::ReusableIndexedVertexBufferObject> m_gpuBufferDatas;
};

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------