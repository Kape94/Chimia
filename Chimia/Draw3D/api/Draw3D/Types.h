#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"

#include "Core/Types.h"

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

class ModelID
{
private:
  friend class Draw3DPrivate;

  ModelID() = delete;
  ModelID(unsigned id)
    : m_id(id)
  {
  }

  unsigned m_id = 0;
};

// ----------------------------------------------------------------------------

class ModelInstanceID
{
private:
  friend class Draw3DPrivate;

  ModelInstanceID() = delete;
  ModelInstanceID(unsigned modelID, unsigned instanceID)
    : m_modelID(modelID)
    , m_instanceID(instanceID)
  {
  }

  unsigned m_modelID = 0;
  unsigned m_instanceID = 0;
};

// ----------------------------------------------------------------------------

class TriangleMeshID
{
private:
  friend class Draw3DPrivate;

  TriangleMeshID() = delete;
  TriangleMeshID(unsigned id)
    : m_id(id)
  {
  }

  unsigned m_id = 0;
};

// ----------------------------------------------------------------------------

class MeshDataView
{
public:
  MeshDataView() = delete;

  MeshDataView(const RawDataView& vertexData,
               const size_t nVertices,
               const RawArrayView& indices)
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