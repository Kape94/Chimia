#include "Renderers.h"

#include "GenericRenderer.h"
#include "ObjectTable.h"
#include "RenderersUtils.h"
#include "Rendering/Shader.h"

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
  const Rendering::Shader& shader,
  void (*setupShaderForTriangleRendering)(const ResourcesGroup&),
  void (*setupShaderForInstancedRendering)(const ResourcesGroup&))
{
  auto [id, renderer] = RenderersInternal::RenderersTable().Insert();

  const VertexLayoutBindingsTemplates bindingTemplates =
    RenderersUtils::GetBindingsTemplatesForLayout(vertexLayout, shader);

  const VertexLayoutDataSchemas dataSchemas =
    RenderersUtils::GetDataSchemasForLayout(vertexLayout);

  renderer->Create(id,
                   dataSchemas.vertexDataLayout,
                   dataSchemas.instancedDataLayout,
                   bindingTemplates.vertexBindingsTemplate,
                   bindingTemplates.instancedBindingsTemplate,
                   setupShaderForTriangleRendering,
                   setupShaderForInstancedRendering);

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