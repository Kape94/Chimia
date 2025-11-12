#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"
#include "MeshDataView.h"

#include "Rendering/ReusableIndexedVertexBufferObject.h"

#include <functional>
#include <vector>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

class Model
{
public:
  void Create(const MeshDataView& meshData);

  void Create(const std::vector<MeshDataView>& meshDatas);

  using BufferHandlerFn =
    std::function<void(const Rendering::ReusableIndexedVertexBufferObject&)>;
  void ForEachBuffer(const BufferHandlerFn& handleBuffer) const;

private:
  void AllocateBufferDataOnGPU(const MeshDataView& bufferData);

  std::vector<Rendering::ReusableIndexedVertexBufferObject> m_gpuBufferDatas;
};

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------