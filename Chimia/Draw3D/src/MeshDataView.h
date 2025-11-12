#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"

#include "Core/Types.h"

#include <vector>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

class MeshDataView
{
public:
  MeshDataView() = delete;

  MeshDataView(const std::vector<float>& vertexData,
               const size_t nVertices,
               const std::vector<unsigned>& indices)
    : m_vertexData(vertexData)
    , m_nVertices(nVertices)
    , m_indices(indices)
  {
  }

  ~MeshDataView() = default;

  MeshDataView(const MeshDataView& other) = delete;
  MeshDataView& operator=(const MeshDataView& other) = delete;

  MeshDataView(MeshDataView&& other) = delete;
  MeshDataView& operator=(MeshDataView&& other) = delete;

  const RawDataView& VertexData() const { return m_vertexData; }
  const RawArrayView& Indices() const { return m_indices; }
  size_t NVertices() const { return m_nVertices; }

private:
  RawDataView m_vertexData;
  size_t m_nVertices = 0;
  RawArrayView m_indices;
};

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------