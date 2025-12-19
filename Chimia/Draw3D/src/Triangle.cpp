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

#include "defaultRenderers/Color3.h"
#include "defaultRenderers/Color3Normal3.h"
#include "defaultRenderers/Color3Normal3TexCoord2.h"
#include "defaultRenderers/Color3TexCoord2.h"
#include "defaultRenderers/Normal3.h"
#include "defaultRenderers/Normal3TexCoord2.h"
#include "defaultRenderers/TexCoord2.h"

#include <vector>

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace {
GenericRenderer* renderer = nullptr;
GenericRenderer* litRenderer = nullptr;
GenericRenderer* litVertexColoredRenderer = nullptr;
GenericRenderer* texturedRenderer = nullptr;
GenericRenderer* texturedLitRenderer = nullptr;
GenericRenderer* coloredTexturedRenderer = nullptr;
GenericRenderer* coloredTexturedLitRenderer = nullptr;

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
}

// ----------------------------------------------------------------------------
// TrianglePrivate
// ----------------------------------------------------------------------------

void
TrianglePrivate::Init()
{
  renderer = &DefaultRenderers::Color3::GetRenderer();
  litRenderer = &DefaultRenderers::Normal3::GetRenderer();
  texturedRenderer = &DefaultRenderers::TexCoord2::GetRenderer();
  texturedLitRenderer = &DefaultRenderers::Normal3TexCoord2::GetRenderer();
  litVertexColoredRenderer = &DefaultRenderers::Color3Normal3::GetRenderer();
  coloredTexturedRenderer = &DefaultRenderers::Color3TexCoord2::GetRenderer();
  coloredTexturedLitRenderer =
    &DefaultRenderers::Color3Normal3TexCoord2::GetRenderer();
}

// ----------------------------------------------------------------------------
// Position3 + Color3
// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangle(const VertexPC& v1,
                         const VertexPC& v2,
                         const VertexPC& v3)
{
  DefaultRenderers::Color3::DrawTriangle(
    v1.position,
    v1.color,
    v2.position,
    v2.color,
    v3.position,
    v3.color,
    ResourceGroupHelper::GetEmptyResource());
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangles(const std::vector<VertexPC>& vertices)
{
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
Chimia::Draw3D::AddStaticTriangles(const std::vector<VertexPC>& vertices)
{
  return renderer->AddStaticTriangles(VectorDataView(vertices),
                                      ResourceGroupHelper::GetEmptyResource());
}

// ----------------------------------------------------------------------------

TriangleMeshID
Chimia::Draw3D::AddStaticTriangles(const std::vector<VertexPC>& vertices,
                                   const std::vector<unsigned>& indices)
{
  const std::vector<VertexPC> unindexedVertex = DropIndices(vertices, indices);
  return AddStaticTriangles(unindexedVertex);
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
  DefaultRenderers::Normal3::DrawTriangle(
    v1.position,
    v1.normal,
    v2.position,
    v2.normal,
    v3.position,
    v3.normal,
    ResourceGroupHelper::GetResourceGroup(material));
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangles(const std::vector<VertexPN>& vertices,
                          const MaterialID& material)
{
  litRenderer->DrawTriangles(VectorArrayView(vertices),
                             ResourceGroupHelper::GetResourceGroup(material));
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
Chimia::Draw3D::AddStaticTriangles(const std::vector<VertexPN>& vertices,
                                   const MaterialID& material)
{
  return litRenderer->AddStaticTriangles(
    VectorDataView(vertices), ResourceGroupHelper::GetResourceGroup(material));
}

// ----------------------------------------------------------------------------

TriangleMeshID
Chimia::Draw3D::AddStaticTriangles(const std::vector<VertexPN>& vertices,
                                   const std::vector<unsigned>& indices,
                                   const MaterialID& material)
{
  const std::vector<VertexPN> unindexedVertex = DropIndices(vertices, indices);
  return AddStaticTriangles(unindexedVertex, material);
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
  DefaultRenderers::TexCoord2::DrawTriangle(
    v1.position,
    v1.texCoord,
    v2.position,
    v2.texCoord,
    v3.position,
    v3.texCoord,
    ResourceGroupHelper::GetResourceGroup(texture));
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangles(const std::vector<VertexPT>& vertices,
                          const TextureID& texture)
{
  texturedRenderer->DrawTriangles(
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
Chimia::Draw3D::AddStaticTriangles(const std::vector<VertexPT>& vertices,
                                   const TextureID& texture)
{
  return texturedRenderer->AddStaticTriangles(
    VectorDataView(vertices), ResourceGroupHelper::GetResourceGroup(texture));
}

// ----------------------------------------------------------------------------

TriangleMeshID
Chimia::Draw3D::AddStaticTriangles(const std::vector<VertexPT>& vertices,
                                   const std::vector<unsigned>& indices,
                                   const TextureID& texture)
{
  const std::vector<VertexPT> unindexedVertex = DropIndices(vertices, indices);
  return AddStaticTriangles(unindexedVertex, texture);
}

// ----------------------------------------------------------------------------
// Position3 + Color3 + Normal3
// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangle(const VertexPCN& v1,
                         const VertexPCN& v2,
                         const VertexPCN& v3)
{
  DefaultRenderers::Color3Normal3::DrawTriangle(
    v1.position,
    v1.color,
    v1.normal,
    v2.position,
    v2.color,
    v2.normal,
    v3.position,
    v3.color,
    v3.normal,
    ResourceGroupHelper::GetEmptyResource());
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangles(const std::vector<VertexPCN>& vertices)
{
  litVertexColoredRenderer->DrawTriangles(
    VectorArrayView(vertices), ResourceGroupHelper::GetEmptyResource());
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
Chimia::Draw3D::AddStaticTriangles(const std::vector<VertexPCN>& vertices)
{
  return litVertexColoredRenderer->AddStaticTriangles(
    VectorDataView(vertices), ResourceGroupHelper::GetEmptyResource());
}

// ----------------------------------------------------------------------------

TriangleMeshID
Chimia::Draw3D::AddStaticTriangles(const std::vector<VertexPCN>& vertices,
                                   const std::vector<unsigned>& indices)
{
  const std::vector<VertexPCN> unindexedVertex = DropIndices(vertices, indices);
  return AddStaticTriangles(unindexedVertex);
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
  DefaultRenderers::Normal3TexCoord2::DrawTriangle(
    v1.position,
    v1.normal,
    v1.texCoord,
    v2.position,
    v2.normal,
    v2.texCoord,
    v3.position,
    v3.normal,
    v3.texCoord,
    ResourceGroupHelper::GetResourceGroup(texture));
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangles(const std::vector<VertexPNT>& vertices,
                          const TextureID& texture)
{
  texturedLitRenderer->DrawTriangles(
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
Chimia::Draw3D::AddStaticTriangles(const std::vector<VertexPNT>& vertices,
                                   const TextureID& texture)
{
  return texturedLitRenderer->AddStaticTriangles(
    VectorDataView(vertices), ResourceGroupHelper::GetResourceGroup(texture));
}

// ----------------------------------------------------------------------------

TriangleMeshID
Chimia::Draw3D::AddStaticTriangles(const std::vector<VertexPNT>& vertices,
                                   const std::vector<unsigned>& indices,
                                   const TextureID& texture)
{
  const std::vector<VertexPNT> unindexedVertex = DropIndices(vertices, indices);
  return AddStaticTriangles(unindexedVertex, texture);
}

// ----------------------------------------------------------------------------
// Position3 + Color3 + TexCoord2
// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangle(const VertexPCT& v1,
                         const VertexPCT& v2,
                         const VertexPCT& v3,
                         const TextureID& texture)
{
  DefaultRenderers::Color3TexCoord2::DrawTriangle(
    v1.position,
    v1.color,
    v1.texCoord,
    v2.position,
    v2.color,
    v2.texCoord,
    v3.position,
    v3.color,
    v3.texCoord,
    ResourceGroupHelper::GetResourceGroup(texture));
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangles(const std::vector<VertexPCT>& vertices,
                          const TextureID& texture)
{
  coloredTexturedRenderer->DrawTriangles(
    VectorArrayView(vertices), ResourceGroupHelper::GetResourceGroup(texture));
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
Chimia::Draw3D::AddStaticTriangles(const std::vector<VertexPCT>& vertices,
                                   const TextureID& texture)
{
  return coloredTexturedRenderer->AddStaticTriangles(
    VectorDataView(vertices), ResourceGroupHelper::GetResourceGroup(texture));
}

// ----------------------------------------------------------------------------

TriangleMeshID
Chimia::Draw3D::AddStaticTriangles(const std::vector<VertexPCT>& vertices,
                                   const std::vector<unsigned>& indices,
                                   const TextureID& texture)
{
  const std::vector<VertexPCT> unindexedVertex = DropIndices(vertices, indices);
  return AddStaticTriangles(unindexedVertex, texture);
}

// ----------------------------------------------------------------------------
// Position3 + Color3 + Normal3 + TexCoord2
// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangle(const VertexPCNT& v1,
                         const VertexPCNT& v2,
                         const VertexPCNT& v3,
                         const TextureID& texture)
{
  DefaultRenderers::Color3Normal3TexCoord2::DrawTriangle(
    v1.position,
    v1.color,
    v1.normal,
    v1.texCoord,
    v2.position,
    v2.color,
    v2.normal,
    v2.texCoord,
    v3.position,
    v3.color,
    v3.normal,
    v3.texCoord,
    ResourceGroupHelper::GetResourceGroup(texture));
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangles(const std::vector<VertexPCNT>& vertices,
                          const TextureID& texture)
{
  coloredTexturedLitRenderer->DrawTriangles(
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
Chimia::Draw3D::AddStaticTriangles(const std::vector<VertexPCNT>& vertices,
                                   const TextureID& texture)
{
  return coloredTexturedLitRenderer->AddStaticTriangles(
    VectorDataView(vertices), ResourceGroupHelper::GetResourceGroup(texture));
}

// ----------------------------------------------------------------------------

TriangleMeshID
Chimia::Draw3D::AddStaticTriangles(const std::vector<VertexPCNT>& vertices,
                                   const std::vector<unsigned>& indices,
                                   const TextureID& texture)
{
  const std::vector<VertexPCNT> unindexedVertex =
    DropIndices(vertices, indices);
  return AddStaticTriangles(unindexedVertex, texture);
}

// ----------------------------------------------------------------------------
// General
// ----------------------------------------------------------------------------

void
Chimia::Draw3D::DeleteStaticTriangles(const TriangleMeshID& meshID)
{
  auto [rendererID, _, __] = Draw3DPrivate::GetTriangleMeshIDValues(meshID);

  if (GenericRenderer* renderer = Renderers::GetRendererByID(rendererID)) {
    renderer->DeleteStaticTriangles(meshID);
  }
}

// ----------------------------------------------------------------------------