#include "Renderers.h"

#include "Core/ObjectRegistry.h"
#include "DataBindingProvider.h"
#include "GenericRenderer.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace RenderersInternal {
Chimia::ObjectRegistry<GenericRenderer>&
RenderersTable()
{
  static Chimia::ObjectRegistry<GenericRenderer> renderersTable;
  return renderersTable;
}
}

// ----------------------------------------------------------------------------

GenericRenderer&
Renderers::CreateRenderer(
  const eVertexLayout& vertexLayout,
  const Rendering::TargetInstance& target,
  void (*setupShaderForTriangleRendering)(const ResourcesGroup&),
  void (*setupShaderForInstancedRendering)(const ResourcesGroup&),
  void (*setupShaderForTransitionRendering)(const ResourcesGroup&))
{
  auto [id, renderer] = RenderersInternal::RenderersTable().Insert();

  const DataBindingProvider dataBindings(vertexLayout, target);

  renderer->Create(id,
                   dataBindings,
                   setupShaderForTriangleRendering,
                   setupShaderForInstancedRendering,
                   setupShaderForTransitionRendering);

  return *renderer;
}

// ----------------------------------------------------------------------------

void
Renderers::DeleteRenderer(GenericRenderer* renderer)
{
  RenderersInternal::RenderersTable().Delete(renderer);
}

// ----------------------------------------------------------------------------

GenericRenderer*
Renderers::GetRendererByID(const unsigned id)
{
  return RenderersInternal::RenderersTable().Find(id);
}

// ----------------------------------------------------------------------------