#include "Renderers.h"

#include "GenericRenderer.h"
#include "ObjectTable.h"
#include "RenderersUtils.h"

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
  const Rendering::TargetInstance& target,
  void (*setupShaderForTriangleRendering)(const ResourcesGroup&),
  void (*setupShaderForInstancedRendering)(const ResourcesGroup&),
  void (*setupShaderForTransitionRendering)(const ResourcesGroup&))
{
  auto [id, renderer] = RenderersInternal::RenderersTable().Insert();

  const VertexLayoutBindingsTemplates bindingTemplates =
    RenderersUtils::GetBindingsTemplatesForLayout(vertexLayout, target);

  const VertexLayoutDataSchemas dataSchemas =
    RenderersUtils::GetDataSchemasForLayout(vertexLayout);

  renderer->Create(id,
                   dataSchemas.vertexDataLayout,
                   dataSchemas.instancedDataLayout,
                   dataSchemas.transitionInstancedDataLayout,
                   bindingTemplates.vertexBindingsTemplate,
                   bindingTemplates.targetVertexBindingsTemplate,
                   bindingTemplates.instancedBindingsTemplate,
                   bindingTemplates.transitionInstancedBindingsTemplate,
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