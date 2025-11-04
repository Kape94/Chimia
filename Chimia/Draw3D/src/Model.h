#pragma once

// ----------------------------------------------------------------------------

#include "BufferData.h"
#include "Draw3DNamespaceDefs.h"

#include "Rendering/ReusableIndexedVertexBufferObject.h"

#include <functional>
#include <vector>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

class Model
{
public:
  void Create(const BufferData& bufferData);

  void Create(const std::vector<BufferData>& bufferDatas);

  using BufferHandlerFn =
    std::function<void(const Rendering::ReusableIndexedVertexBufferObject&)>;
  void ForEachBuffer(const BufferHandlerFn& handleBuffer) const;

private:
  void AllocateBufferDataOnGPU(const BufferData& bufferData);

  std::vector<Rendering::ReusableIndexedVertexBufferObject> m_gpuBufferDatas;
};

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------