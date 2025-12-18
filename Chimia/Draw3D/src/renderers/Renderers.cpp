#include "Renderers.h"

#include "GenericRenderer.h"
#include "ObjectTable.h"
#include "VertexLayoutAttributes.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace RenderersInternal {
ObjectTable<GenericRenderer>&
RenderersTable()
{
  static ObjectTable<GenericRenderer> renderersTable;
  return renderersTable;
}
}

// ----------------------------------------------------------------------------

GenericRenderer&
Renderers::CreateRenderer(
  const eVertexLayout& vertexLayout,
  void (*setupShaderForTriangleRendering)(const ResourcesGroup&),
  void (*setupShaderForInstancedRendering)(const ResourcesGroup&))
{
  auto [id, renderer] = RenderersInternal::RenderersTable().Insert();

  const VertexLayoutAttributes layoutAttributes =
    GetAttributesForLayout(vertexLayout);

  renderer->Create(id,
                   layoutAttributes.vertexAttributes,
                   layoutAttributes.instancedAttributes,
                   setupShaderForTriangleRendering,
                   setupShaderForInstancedRendering);

  return *renderer;
}

// ----------------------------------------------------------------------------

GenericRenderer*
Renderers::GetRendererByID(const unsigned id)
{
  return RenderersInternal::RenderersTable().Find(id);
}

// ----------------------------------------------------------------------------