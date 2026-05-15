#include "Triangle.h"
#include "TrianglePrivate.h"

// ----------------------------------------------------------------------------

#include "Core/Types.h"
#include "Draw3DNamespaceDefs.h"
#include "Draw3DPrivate.h"
#include "GenericRenderer.h"
#include "Renderers.h"
#include "ResourceGroupHelper.h"
#include "Types.h"

#include "defaultRenderers/Color4.h"
#include "defaultRenderers/Color4Normal3.h"
#include "defaultRenderers/Color4Normal3TexCoord2.h"
#include "defaultRenderers/Color4TexCoord2.h"
#include "defaultRenderers/Normal3.h"
#include "defaultRenderers/Normal3TexCoord2.h"
#include "defaultRenderers/TexCoord2.h"

#include <vector>

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace {
GenericRenderer* color4Renderer = nullptr;
GenericRenderer* normal3Renderer = nullptr;
GenericRenderer* color4Normal3Renderer = nullptr;
GenericRenderer* texCoord2Renderer = nullptr;
GenericRenderer* normal3TexCoord2Renderer = nullptr;
GenericRenderer* color4TexCoord2Renderer = nullptr;
GenericRenderer* color4Normal3TexCoord2Renderer = nullptr;

GenericRenderer* color4TransparentRenderer = nullptr;
GenericRenderer* color4TexCoord2TransparentRenderer = nullptr;

constexpr size_t POS3_SIZE = sizeof(glm::vec3);
constexpr size_t COLOR4_SIZE = sizeof(glm::vec4);
constexpr size_t NORM3_SIZE = sizeof(glm::vec3);
constexpr size_t TEX_COORD2_SIZE = sizeof(glm::vec2);

template<class VertexType>
std::vector<VertexType>
DropIndices(const std::vector<VertexType>& vertexData,
            const std::vector<unsigned>& indices)
{
  std::vector<VertexType> unindexedData;
  unindexedData.reserve(indices.size());

  for (const unsigned i : indices) {
    unindexedData.push_back(vertexData[i]);
  }

  return unindexedData;
}

template<typename T>
Chimia::RawArrayView
VectorArrayView(const std::vector<T>& vector)
{
  return { vector.data(), vector.size(), sizeof(T) };
}

template<typename T>
Chimia::RawDataView
VectorDataView(const std::vector<T>& vector)
{
  return { vector.data(), vector.size() * sizeof(T) };
}

template<typename Vertex>
bool
IsTransparent(const Vertex& v)
{
  return v.color.a < 0.98f;
}

template<typename Vertex>
bool
IsTransparent(const Vertex& v1, const Vertex& v2, const Vertex& v3)
{
  return IsTransparent(v1) || IsTransparent(v2) || IsTransparent(v3);
}

template<typename Vertex>
bool
IsTransparent(const std::vector<Vertex>& vertices)
{
  for (const Vertex& v : vertices) {
    if (IsTransparent(v)) {
      return true;
    }
  }
  return false;
}

GenericRenderer*
GetColor4Renderer(const bool isTransparent)
{
  return isTransparent ? color4TransparentRenderer : color4Renderer;
}

GenericRenderer*
GetColor4TexCoord2Renderer(const bool isTransparent)
{
  return isTransparent ? color4TexCoord2TransparentRenderer
                       : color4TexCoord2Renderer;
}
}

// ----------------------------------------------------------------------------
// TrianglePrivate
// ----------------------------------------------------------------------------

void
TrianglePrivate::Init()
{
  color4Renderer = &DefaultRenderers::Color4::GetRenderer();
  normal3Renderer = &DefaultRenderers::Normal3::GetRenderer();
  texCoord2Renderer = &DefaultRenderers::TexCoord2::GetRenderer();
  normal3TexCoord2Renderer = &DefaultRenderers::Normal3TexCoord2::GetRenderer();
  color4Normal3Renderer = &DefaultRenderers::Color4Normal3::GetRenderer();
  color4TexCoord2Renderer = &DefaultRenderers::Color4TexCoord2::GetRenderer();
  color4Normal3TexCoord2Renderer =
    &DefaultRenderers::Color4Normal3TexCoord2::GetRenderer();

  color4TransparentRenderer =
    &DefaultRenderers::Color4::GetTransparentRenderer();
  color4TexCoord2TransparentRenderer =
    &DefaultRenderers::Color4TexCoord2::GetTransparentRenderer();
}

// ----------------------------------------------------------------------------
// Position3 + Color4
// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangle(const VertexPC& v1,
                         const VertexPC& v2,
                         const VertexPC& v3)
{
  const bool isTransparent = IsTransparent(v1, v2, v3);
  GenericRenderer* renderer = GetColor4Renderer(isTransparent);

  renderer->DrawTriangle({ { &v1.position, POS3_SIZE },
                           { &v1.color, COLOR4_SIZE },
                           { &v2.position, POS3_SIZE },
                           { &v2.color, COLOR4_SIZE },
                           { &v3.position, POS3_SIZE },
                           { &v3.color, COLOR4_SIZE } },
                         ResourceGroupHelper::GetEmptyResource());
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangles(const std::vector<VertexPC>& vertices)
{
  const bool isTransparent = IsTransparent(vertices);
  GenericRenderer* renderer = GetColor4Renderer(isTransparent);

  renderer->DrawTriangles(VectorArrayView(vertices),
                          ResourceGroupHelper::GetEmptyResource());
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangles(const std::vector<VertexPC>& vertices,
                          const std::vector<unsigned>& indices)
{
  const std::vector<VertexPC> unindexedVertex = DropIndices(vertices, indices);
  Triangles(unindexedVertex);
}

// ----------------------------------------------------------------------------

TriangleMeshID
Chimia::Draw3D::AddRetainedTriangles(const std::vector<VertexPC>& vertices)
{
  const bool isTransparent = IsTransparent(vertices);
  GenericRenderer* renderer = GetColor4Renderer(isTransparent);

  return renderer->AddRetainedTriangles(
    VectorDataView(vertices), ResourceGroupHelper::GetEmptyResource());
}

// ----------------------------------------------------------------------------

TriangleMeshID
Chimia::Draw3D::AddRetainedTriangles(const std::vector<VertexPC>& vertices,
                                     const std::vector<unsigned>& indices)
{
  const std::vector<VertexPC> unindexedVertex = DropIndices(vertices, indices);
  return AddRetainedTriangles(unindexedVertex);
}

// ----------------------------------------------------------------------------
// Position3 + Normal3
// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangle(const VertexPN& v1,
                         const VertexPN& v2,
                         const VertexPN& v3,
                         const MaterialID& material)
{
  normal3Renderer->DrawTriangle(
    {
      { &v1.position, POS3_SIZE },
      { &v1.normal, NORM3_SIZE },
      { &v2.position, POS3_SIZE },
      { &v2.normal, NORM3_SIZE },
      { &v3.position, POS3_SIZE },
      { &v3.normal, NORM3_SIZE },
    },
    ResourceGroupHelper::GetResourceGroup(material));
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangles(const std::vector<VertexPN>& vertices,
                          const MaterialID& material)
{
  normal3Renderer->DrawTriangles(
    VectorArrayView(vertices), ResourceGroupHelper::GetResourceGroup(material));
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangles(const std::vector<VertexPN>& vertices,
                          const std::vector<unsigned>& indices,
                          const MaterialID& material)
{
  const std::vector<VertexPN> unindexedVertex = DropIndices(vertices, indices);
  Triangles(unindexedVertex, material);
}

// ----------------------------------------------------------------------------

TriangleMeshID
Chimia::Draw3D::AddRetainedTriangles(const std::vector<VertexPN>& vertices,
                                     const MaterialID& material)
{
  return normal3Renderer->AddRetainedTriangles(
    VectorDataView(vertices), ResourceGroupHelper::GetResourceGroup(material));
}

// ----------------------------------------------------------------------------

TriangleMeshID
Chimia::Draw3D::AddRetainedTriangles(const std::vector<VertexPN>& vertices,
                                     const std::vector<unsigned>& indices,
                                     const MaterialID& material)
{
  const std::vector<VertexPN> unindexedVertex = DropIndices(vertices, indices);
  return AddRetainedTriangles(unindexedVertex, material);
}

// ----------------------------------------------------------------------------
// Position3 + TexCoord2
// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangle(const VertexPT& v1,
                         const VertexPT& v2,
                         const VertexPT& v3,
                         const TextureID& texture)
{
  texCoord2Renderer->DrawTriangle(
    {
      { &v1.position, POS3_SIZE },
      { &v1.texCoord, TEX_COORD2_SIZE },
      { &v2.position, POS3_SIZE },
      { &v2.texCoord, TEX_COORD2_SIZE },
      { &v3.position, POS3_SIZE },
      { &v3.texCoord, TEX_COORD2_SIZE },
    },
    ResourceGroupHelper::GetResourceGroup(texture));
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangles(const std::vector<VertexPT>& vertices,
                          const TextureID& texture)
{
  texCoord2Renderer->DrawTriangles(
    VectorArrayView(vertices), ResourceGroupHelper::GetResourceGroup(texture));
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangles(const std::vector<VertexPT>& vertices,
                          const std::vector<unsigned>& indices,
                          const TextureID& texture)
{
  const std::vector<VertexPT> unindexedVertex = DropIndices(vertices, indices);
  Triangles(unindexedVertex, texture);
}

// ----------------------------------------------------------------------------

TriangleMeshID
Chimia::Draw3D::AddRetainedTriangles(const std::vector<VertexPT>& vertices,
                                     const TextureID& texture)
{
  return texCoord2Renderer->AddRetainedTriangles(
    VectorDataView(vertices), ResourceGroupHelper::GetResourceGroup(texture));
}

// ----------------------------------------------------------------------------

TriangleMeshID
Chimia::Draw3D::AddRetainedTriangles(const std::vector<VertexPT>& vertices,
                                     const std::vector<unsigned>& indices,
                                     const TextureID& texture)
{
  const std::vector<VertexPT> unindexedVertex = DropIndices(vertices, indices);
  return AddRetainedTriangles(unindexedVertex, texture);
}

// ----------------------------------------------------------------------------
// Position3 + Color4 + Normal3
// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangle(const VertexPCN& v1,
                         const VertexPCN& v2,
                         const VertexPCN& v3)
{
  color4Normal3Renderer->DrawTriangle(
    {
      { &v1.position, POS3_SIZE },
      { &v1.color, COLOR4_SIZE },
      { &v1.normal, NORM3_SIZE },
      { &v2.position, POS3_SIZE },
      { &v2.color, COLOR4_SIZE },
      { &v2.normal, NORM3_SIZE },
      { &v3.position, POS3_SIZE },
      { &v3.color, COLOR4_SIZE },
      { &v3.normal, NORM3_SIZE },
    },
    ResourceGroupHelper::GetEmptyResource());
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangles(const std::vector<VertexPCN>& vertices)
{
  color4Normal3Renderer->DrawTriangles(VectorArrayView(vertices),
                                       ResourceGroupHelper::GetEmptyResource());
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangles(const std::vector<VertexPCN>& vertices,
                          const std::vector<unsigned>& indices)
{
  const std::vector<VertexPCN> unindexedVertex = DropIndices(vertices, indices);
  Triangles(unindexedVertex);
}

// ----------------------------------------------------------------------------

TriangleMeshID
Chimia::Draw3D::AddRetainedTriangles(const std::vector<VertexPCN>& vertices)
{
  return color4Normal3Renderer->AddRetainedTriangles(
    VectorDataView(vertices), ResourceGroupHelper::GetEmptyResource());
}

// ----------------------------------------------------------------------------

TriangleMeshID
Chimia::Draw3D::AddRetainedTriangles(const std::vector<VertexPCN>& vertices,
                                     const std::vector<unsigned>& indices)
{
  const std::vector<VertexPCN> unindexedVertex = DropIndices(vertices, indices);
  return AddRetainedTriangles(unindexedVertex);
}

// ----------------------------------------------------------------------------
// Position3 + Normal3 + TexCoord2
// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangle(const VertexPNT& v1,
                         const VertexPNT& v2,
                         const VertexPNT& v3,
                         const TextureID& texture)
{
  normal3TexCoord2Renderer->DrawTriangle(
    {
      { &v1.position, POS3_SIZE },
      { &v1.normal, NORM3_SIZE },
      { &v1.texCoord, TEX_COORD2_SIZE },
      { &v2.position, POS3_SIZE },
      { &v2.normal, NORM3_SIZE },
      { &v2.texCoord, TEX_COORD2_SIZE },
      { &v3.position, POS3_SIZE },
      { &v3.normal, NORM3_SIZE },
      { &v3.texCoord, TEX_COORD2_SIZE },
    },
    ResourceGroupHelper::GetResourceGroup(texture));
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangles(const std::vector<VertexPNT>& vertices,
                          const TextureID& texture)
{
  normal3TexCoord2Renderer->DrawTriangles(
    VectorArrayView(vertices), ResourceGroupHelper::GetResourceGroup(texture));
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangles(const std::vector<VertexPNT>& vertices,
                          const std::vector<unsigned>& indices,
                          const TextureID& texture)
{
  const std::vector<VertexPNT> unindexedVertex = DropIndices(vertices, indices);
  Triangles(unindexedVertex, texture);
}

// ----------------------------------------------------------------------------

TriangleMeshID
Chimia::Draw3D::AddRetainedTriangles(const std::vector<VertexPNT>& vertices,
                                     const TextureID& texture)
{
  return normal3TexCoord2Renderer->AddRetainedTriangles(
    VectorDataView(vertices), ResourceGroupHelper::GetResourceGroup(texture));
}

// ----------------------------------------------------------------------------

TriangleMeshID
Chimia::Draw3D::AddRetainedTriangles(const std::vector<VertexPNT>& vertices,
                                     const std::vector<unsigned>& indices,
                                     const TextureID& texture)
{
  const std::vector<VertexPNT> unindexedVertex = DropIndices(vertices, indices);
  return AddRetainedTriangles(unindexedVertex, texture);
}

// ----------------------------------------------------------------------------
// Position3 + Color4 + TexCoord2
// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangle(const VertexPCT& v1,
                         const VertexPCT& v2,
                         const VertexPCT& v3,
                         const TextureID& texture)
{
  const bool isTransparent = IsTransparent(v1, v2, v3);
  GenericRenderer* renderer = GetColor4TexCoord2Renderer(isTransparent);

  renderer->DrawTriangle({ { &v1.position, POS3_SIZE },
                           { &v1.color, COLOR4_SIZE },
                           { &v1.texCoord, TEX_COORD2_SIZE },
                           { &v2.position, POS3_SIZE },
                           { &v2.color, COLOR4_SIZE },
                           { &v2.texCoord, TEX_COORD2_SIZE },
                           { &v3.position, POS3_SIZE },
                           { &v3.color, COLOR4_SIZE },
                           { &v3.texCoord, TEX_COORD2_SIZE } },
                         ResourceGroupHelper::GetResourceGroup(texture));
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangles(const std::vector<VertexPCT>& vertices,
                          const TextureID& texture)
{
  const bool isTransparent = IsTransparent(vertices);
  GenericRenderer* renderer = GetColor4TexCoord2Renderer(isTransparent);

  renderer->DrawTriangles(VectorArrayView(vertices),
                          ResourceGroupHelper::GetResourceGroup(texture));
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangles(const std::vector<VertexPCT>& vertices,
                          const std::vector<unsigned>& indices,
                          const TextureID& texture)
{
  const std::vector<VertexPCT> unindexedVertex = DropIndices(vertices, indices);
  Triangles(unindexedVertex, texture);
}

// ----------------------------------------------------------------------------

TriangleMeshID
Chimia::Draw3D::AddRetainedTriangles(const std::vector<VertexPCT>& vertices,
                                     const TextureID& texture)
{
  const bool isTransparent = IsTransparent(vertices);
  GenericRenderer* renderer = GetColor4TexCoord2Renderer(isTransparent);

  return renderer->AddRetainedTriangles(
    VectorDataView(vertices), ResourceGroupHelper::GetResourceGroup(texture));
}

// ----------------------------------------------------------------------------

TriangleMeshID
Chimia::Draw3D::AddRetainedTriangles(const std::vector<VertexPCT>& vertices,
                                     const std::vector<unsigned>& indices,
                                     const TextureID& texture)
{
  const std::vector<VertexPCT> unindexedVertex = DropIndices(vertices, indices);
  return AddRetainedTriangles(unindexedVertex, texture);
}

// ----------------------------------------------------------------------------
// Position3 + Color4 + Normal3 + TexCoord2
// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangle(const VertexPCNT& v1,
                         const VertexPCNT& v2,
                         const VertexPCNT& v3,
                         const TextureID& texture)
{
  color4Normal3TexCoord2Renderer->DrawTriangle(
    {
      { &v1.position, POS3_SIZE },
      { &v1.color, COLOR4_SIZE },
      { &v1.normal, NORM3_SIZE },
      { &v1.texCoord, TEX_COORD2_SIZE },
      { &v2.position, POS3_SIZE },
      { &v2.color, COLOR4_SIZE },
      { &v2.normal, NORM3_SIZE },
      { &v2.texCoord, TEX_COORD2_SIZE },
      { &v3.position, POS3_SIZE },
      { &v3.color, COLOR4_SIZE },
      { &v3.normal, NORM3_SIZE },
      { &v3.texCoord, TEX_COORD2_SIZE },
    },
    ResourceGroupHelper::GetResourceGroup(texture));
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangles(const std::vector<VertexPCNT>& vertices,
                          const TextureID& texture)
{
  color4Normal3TexCoord2Renderer->DrawTriangles(
    VectorArrayView(vertices), ResourceGroupHelper::GetResourceGroup(texture));
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangles(const std::vector<VertexPCNT>& vertices,
                          const std::vector<unsigned>& indices,
                          const TextureID& texture)
{
  const std::vector<VertexPCNT> unindexedVertex =
    DropIndices(vertices, indices);
  Triangles(unindexedVertex, texture);
}

// ----------------------------------------------------------------------------

TriangleMeshID
Chimia::Draw3D::AddRetainedTriangles(const std::vector<VertexPCNT>& vertices,
                                     const TextureID& texture)
{
  return color4Normal3TexCoord2Renderer->AddRetainedTriangles(
    VectorDataView(vertices), ResourceGroupHelper::GetResourceGroup(texture));
}

// ----------------------------------------------------------------------------

TriangleMeshID
Chimia::Draw3D::AddRetainedTriangles(const std::vector<VertexPCNT>& vertices,
                                     const std::vector<unsigned>& indices,
                                     const TextureID& texture)
{
  const std::vector<VertexPCNT> unindexedVertex =
    DropIndices(vertices, indices);
  return AddRetainedTriangles(unindexedVertex, texture);
}

// ----------------------------------------------------------------------------
// General
// ----------------------------------------------------------------------------

void
Chimia::Draw3D::DeleteRetainedTriangles(const TriangleMeshID& meshID)
{
  auto [rendererID, _, __] = Draw3DPrivate::GetTriangleMeshIDValues(meshID);

  if (GenericRenderer* renderer = Renderers::GetRendererByID(rendererID)) {
    renderer->DeleteRetainedTriangles(meshID);
  }
}

// ----------------------------------------------------------------------------