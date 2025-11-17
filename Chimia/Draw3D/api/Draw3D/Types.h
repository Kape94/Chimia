#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"

#include "Core/Types.h"

#include <glm/vec3.hpp>

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

class LitTriangleMeshID
{
private:
  friend class Draw3DPrivate;

  LitTriangleMeshID() = delete;
  LitTriangleMeshID(unsigned id, unsigned materialID)
    : m_id(id)
    , m_materialID(materialID)
  {
  }

  unsigned m_id = 0;
  unsigned m_materialID = 0;
};

// ----------------------------------------------------------------------------

class MaterialID
{
private:
  friend class Draw3DPrivate;

  MaterialID() = delete;
  MaterialID(unsigned id)
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

struct LightColor
{
  glm::vec3 ambient{ 0.0f, 0.0f, 0.0f };
  glm::vec3 diffuse{ 0.0f, 0.0f, 0.0f };
  glm::vec3 specular{ 0.0f, 0.0f, 0.0f };
};

struct PointLightAttenuation
{
  float quadratic = 0.0f;
  float linear = 0.0f;
  float constant = 0.0f;
};

struct DirectionalLight
{
  glm::vec3 direction{ 0.0f, 0.0f, 0.0f };
  LightColor color;
};

struct PointLight
{
  glm::vec3 position{ 0.0f, 0.0f, 0.0f };
  LightColor color;
  PointLightAttenuation attenuation;
};

struct Material
{
  glm::vec3 ambient{ 0.0f, 0.0f, 0.0f };
  glm::vec3 diffuse{ 0.0f, 0.0f, 0.0f };
  glm::vec3 specular{ 0.0f, 0.0f, 0.0f };
  float shininess;
};

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------