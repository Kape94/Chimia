#include "VertexColoredRendererImpl.h"

#include "CameraPrivate.h"
#include "GenericRenderer.h"
#include "ResourceGroup.h"
#include "Shaders.h"

#include "Rendering/Shader.h"
#include "Types.h"
#include "eRendererType.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace {

void
ConfigureForTriangleDrawing(const ResourcesGroup& resource)
{
  Chimia::Rendering::Shader& shader = Shaders::VertexColored();
  shader.Use();
  CameraPrivate::SetCameraOnShader(shader);
}

void
ConfigureForInstancedDrawing(const ResourcesGroup& resource)
{
  Chimia::Rendering::Shader& shader =
    Shaders::VertexColoredWithInstancedTransform();
  shader.Use();
  CameraPrivate::SetCameraOnShader(shader);
}

}

// ----------------------------------------------------------------------------

GenericRenderer&
VertexColoredRendererImpl::GetRenderer()
{
  constexpr unsigned RENDERER_ID =
    static_cast<unsigned>(eRendererType::VERTEX_COLORED);

  static GenericRenderer renderer(RENDERER_ID,
                                  eVertexLayout::POSITION3_COLOR3,
                                  ConfigureForTriangleDrawing,
                                  ConfigureForInstancedDrawing);
  return renderer;
}

// ----------------------------------------------------------------------------

void
VertexColoredRendererImpl::DrawTriangle(const glm::vec3& p1,
                                        const glm::vec3& color1,
                                        const glm::vec3& p2,
                                        const glm::vec3& color2,
                                        const glm::vec3& p3,
                                        const glm::vec3& color3,
                                        const ResourceGroupID& resource)
{
  constexpr size_t POS3_SIZE = sizeof(glm::vec3);
  constexpr size_t COL3_SIZE = sizeof(glm::vec3);

  auto& renderer = GetRenderer();
  renderer.DrawTriangle({ { &p1, POS3_SIZE },
                          { &color1, COL3_SIZE },
                          { &p2, POS3_SIZE },
                          { &color2, COL3_SIZE },
                          { &p3, POS3_SIZE },
                          { &color3, COL3_SIZE } },
                        resource);
}

// ----------------------------------------------------------------------------
