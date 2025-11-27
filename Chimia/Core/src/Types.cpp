#include "Types.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_CORE_NAMESPACE

// ----------------------------------------------------------------------------

namespace TypesUtils {

size_t
ComputeNFloatsCountForAttribute(const MeshAttributes& attributes)
{
  size_t nFloatsPerVertex = 3;                            // Position3
  nFloatsPerVertex += attributes.hasVertexColor ? 4 : 0;  // Color4
  nFloatsPerVertex += attributes.hasVertexNormal ? 3 : 0; // Normal3
  nFloatsPerVertex += attributes.hasTexCoords ? 2 : 0;    // TexCoord2

  return nFloatsPerVertex;
}

void
PushIntoVector(const glm::vec3& pos, std::vector<float>& vector)
{
  vector.insert(vector.end(), { pos.x, pos.y, pos.z });
}

void
PushIntoVector(const glm::vec4& col,
               const bool shouldIncludeAlpha,
               std::vector<float>& vector)
{
  if (shouldIncludeAlpha) {
    vector.insert(vector.end(), { col.r, col.g, col.b, col.a });
  } else {
    vector.insert(vector.end(), { col.r, col.g, col.b });
  }
}

void
PushIntoVector(const glm::vec2& coord, std::vector<float>& vector)
{
  vector.insert(vector.end(), { coord.x, coord.y });
}

MeshAttributes
ResultingAttributes(const Mesh& mesh, const MeshAttributes& requested)
{
  const MeshAttributes meshAttributes = mesh.GetMeshAttributes();

  MeshAttributes resulting;
  resulting.hasVertexColor =
    requested.hasVertexColor && meshAttributes.hasVertexColor;
  resulting.hasVertexNormal =
    requested.hasVertexNormal && meshAttributes.hasVertexNormal;
  resulting.hasTexCoords =
    requested.hasTexCoords && meshAttributes.hasTexCoords;
  resulting.includeColorAlpha = requested.includeColorAlpha;

  return resulting;
}

}

// ----------------------------------------------------------------------------
// Mesh
// ----------------------------------------------------------------------------

void
Mesh::AddPosition(const glm::vec3& pos)
{
  m_positions.push_back(pos);
  m_shouldRebuildRenderData = true;
}

// ----------------------------------------------------------------------------

void
Mesh::AddColor(const glm::vec4& col)
{
  m_colors.push_back(col);
  m_shouldRebuildRenderData = true;
}

// ----------------------------------------------------------------------------

void
Mesh::AddNormal(const glm::vec3& normal)
{
  m_normals.push_back(normal);
  m_shouldRebuildRenderData = true;
}

// ----------------------------------------------------------------------------

void
Mesh::AddTexCoord(const glm::vec2& texCoord)
{
  m_texCoords.push_back(texCoord);
  m_shouldRebuildRenderData = true;
}

// ----------------------------------------------------------------------------

void
Mesh::AddIndex(const unsigned i)
{
  m_indices.push_back(i);
  m_shouldRebuildRenderData = true;
}

// ----------------------------------------------------------------------------

MeshAttributes
Mesh::GetMeshAttributes() const
{
  MeshAttributes attributes;
  attributes.hasVertexColor = !m_colors.empty();
  attributes.hasVertexNormal = !m_normals.empty();
  attributes.hasTexCoords = !m_texCoords.empty();

  return attributes;
}

// ----------------------------------------------------------------------------

const MeshBufferData&
Mesh::GetRenderData() const
{
  if (m_shouldRebuildRenderData) {
    BuildRenderData();
  }

  return m_renderData;
}

// ----------------------------------------------------------------------------

void
Mesh::BuildRenderData() const
{
  m_renderData = MeshBufferData();

  const MeshAttributes attributes = GetMeshAttributes();

  const size_t nFloatsPerVertex =
    TypesUtils::ComputeNFloatsCountForAttribute(attributes);
  const size_t nVertices = m_positions.size();

  std::vector<float>& vertexData = m_renderData.vertexDataValues;
  vertexData.reserve(nVertices * nFloatsPerVertex);

  for (size_t i = 0; i < nVertices; ++i) {
    TypesUtils::PushIntoVector(m_positions[i], vertexData);
    if (attributes.hasVertexColor) {
      TypesUtils::PushIntoVector(
        m_colors[i], attributes.includeColorAlpha, vertexData);
    }
    if (attributes.hasVertexNormal) {
      TypesUtils::PushIntoVector(m_normals[i], vertexData);
    }
    if (attributes.hasTexCoords) {
      TypesUtils::PushIntoVector(m_texCoords[i], vertexData);
    }
  }

  m_renderData.indices = m_indices;
  m_renderData.attributes = attributes;
  m_renderData.nVertices = nVertices;

  m_shouldRebuildRenderData = false;
}

// ----------------------------------------------------------------------------
