#pragma once

#include "Draw3DNamespaceDefs.h"
#include <vector>

BEGIN_CHIMIA_DRAW3D_NAMESPACE

class BufferData
{
public:
  BufferData() = delete;

  BufferData(const std::vector<float>& vertexData,
             const std::vector<unsigned>& indices)
    : m_vertexData(vertexData)
    , m_indices(indices)
  {
  }

  ~BufferData() = default;

  BufferData(const BufferData& other) = delete;
  BufferData& operator=(const BufferData& other) = delete;

  BufferData(BufferData&& other) = delete;
  BufferData& operator=(BufferData&& other) = delete;

  const std::vector<float>& VertexData() const { return m_vertexData; }
  const std::vector<unsigned>& Indices() const { return m_indices; }

private:
  std::vector<float> m_vertexData;
  std::vector<unsigned> m_indices;
};

END_CHIMIA_DRAW3D_NAMESPACE