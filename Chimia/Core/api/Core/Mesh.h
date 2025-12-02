#pragma once

// ----------------------------------------------------------------------------

#include "CoreNamespaceDefs.h"
#include "DataBuffer.h"
#include "Types.h"

#include <functional>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_CORE_NAMESPACE

// ----------------------------------------------------------------------------

struct VertexView
{
  glm::vec3* position = nullptr;
  glm::vec4* color = nullptr;
  glm::vec3* normal = nullptr;
  glm::vec2* texCoord = nullptr;
};

struct MeshAttributes
{
  bool hasVertexColor = false;
  bool hasVertexNormal = false;
  bool hasTexCoords = false;

  // User can request colors without alpha
  bool includeColorAlpha = true;
};

struct MeshBufferData
{
  std::vector<float> vertexDataValues;
  std::vector<unsigned> indices;
  MeshAttributes attributes;
  size_t nVertices = 0;

  bool HasIndices() const { return indices.size() > 0; }
};

// ----------------------------------------------------------------------------

class Mesh
{
public:
  Mesh() = default;

  Mesh(const Mesh&) = delete;
  Mesh& operator=(const Mesh&) = delete;

  Mesh(Mesh&&);
  Mesh& operator=(Mesh&&);

  void AddPosition(const glm::vec3& pos);
  void AddColor(const glm::vec4& col);
  void AddNormal(const glm::vec3& normal);
  void AddTexCoord(const glm::vec2& texCoord);
  void AddIndex(const unsigned i);

  MeshAttributes GetMeshAttributes() const;
  const MeshBufferData& GetRenderData() const;

  void SetCustomData(const RawDataView& data);

  const void* GetCustomData() const;
  void* GetCustomData();

  void ForEachVertex(const std::function<void(VertexView)>& action);

private:
  void BuildRenderData() const;

  mutable bool m_shouldRebuildRenderData = true;
  mutable MeshBufferData m_renderData;

  DataBuffer m_customData;

  std::vector<glm::vec3> m_positions;
  std::vector<glm::vec4> m_colors;
  std::vector<glm::vec3> m_normals;
  std::vector<glm::vec2> m_texCoords;
  std::vector<unsigned> m_indices;
};

// ----------------------------------------------------------------------------

END_CHIMIA_CORE_NAMESPACE

// ----------------------------------------------------------------------------