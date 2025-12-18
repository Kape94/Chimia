#include "Renderers.h"

#include "GenericRenderer.h"
#include "ObjectTable.h"
#include "VertexLayoutAttributes.h"

#include <memory>

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace RenderersInternal {
std::unique_ptr<ObjectTable<GenericRenderer>> g_renderers = nullptr;

ObjectTable<GenericRenderer>&
RenderersTable()
{
  if (g_renderers == nullptr) {
    g_renderers.reset(new ObjectTable<GenericRenderer>);
  }

  return *g_renderers;
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