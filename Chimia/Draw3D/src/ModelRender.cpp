#include "ModelRender.h"

#include "Core/Types.h"
#include "eRendererType.h"

#include "LitRendererImpl.h"
#include "VertexColoredRendererImpl.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace {
eRendererType
SelectRenderer(const Chimia::MeshAttributes& attributes)
{
  if (attributes.hasVertexColor && !attributes.hasVertexNormal &&
      !attributes.hasTexCoords) {
    return eRendererType::VERTEX_COLORED;
  } else if (!attributes.hasVertexColor && attributes.hasVertexNormal &&
             !attributes.hasTexCoords) {
    return eRendererType::LIT;
  }
  return eRendererType::NONE;
}

auto& vertexColored = VertexColoredRendererImpl::getInstance();
auto& lit = LitRendererImpl::getInstance();
}

// ----------------------------------------------------------------------------

void
CHIMIA_DRAW3D_NAMESPACE_NAME::DrawTriangleMesh(const MeshBufferData& mesh)
{
  switch (SelectRenderer(mesh.attributes)) {
    case Chimia::Draw3D::eRendererType::VERTEX_COLORED:
      if (mesh.HasIndices()) {
        vertexColored.DrawTriangles(mesh.vertexDataValues, mesh.indices);
      } else {
        vertexColored.DrawTriangles(mesh.vertexDataValues);
      }
    case Chimia::Draw3D::eRendererType::LIT:
      // Log error: missing material
    case Chimia::Draw3D::eRendererType::NONE:
    default:
      // Log error: didn't find any suitable renderer
  }
}