#include "LitWithVertexColorRendererImpl.h"

#include "Config.h"
#include "GenericRenderer.h"
#include "IlluminationPrivate.h"
#include "Shaders.h"

#include "Rendering/Shader.h"
#include "Rendering/ShaderAttribute.h"
#include "Types.h"
#include "eRendererType.h"
#include <glm/ext/vector_float3.hpp>

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace {

Chimia::Rendering::Shader&
GetShaderForTriangleMeshDrawing()
{
  return Chimia::Draw3D::Config::IlluminationModel() ==
             eIlluminationModel::GOURAUD
           ? Chimia::Draw3D::Shaders::GouraudLitWithVertexColor()
           : Chimia::Draw3D::Shaders::PhongLitWithVertexColor();
}

Chimia::Rendering::Shader&
GetShaderForModelDrawing()
{
  return Config::IlluminationModel() == eIlluminationModel::GOURAUD
           ? Chimia::Draw3D::Shaders::
               GouraudLitWithInstancedTransformAndVertexColor()
           : Chimia::Draw3D::Shaders::
               PhongLitWithInstancedTransformAndVertexColor();
}

void
ConfigureShaderForTriangleDrawing(const ResourcesGroup&)
{
  Chimia::Rendering::Shader& shader = GetShaderForTriangleMeshDrawing();

  shader.Use();
  IlluminationPrivate::ConfigureLightsOnShader(shader);
}

void
ConfigureShaderForTransformedModelDrawing(const ResourcesGroup&)
{
  Chimia::Rendering::Shader& shader = GetShaderForModelDrawing();

  shader.Use();
  IlluminationPrivate::ConfigureLightsOnShader(shader);
}
}

// ----------------------------------------------------------------------------

GenericRenderer&
LitWithVertexColorRendererImpl::GetRenderer()
{
  constexpr unsigned RENDERER_ID =
    static_cast<unsigned>(eRendererType::VERTEX_COLORED_LIT);

  static GenericRenderer renderer(RENDERER_ID,
                                  eVertexLayout::POSITION3_COLOR3_NORMAL3,
                                  ConfigureShaderForTriangleDrawing,
                                  ConfigureShaderForTransformedModelDrawing);
  return renderer;
}

// ----------------------------------------------------------------------------

void
LitWithVertexColorRendererImpl::DrawTriangle(const glm::vec3& p1,
                                             const glm::vec3& p1Color,
                                             const glm::vec3& p1Normal,
                                             const glm::vec3& p2,
                                             const glm::vec3& p2Color,
                                             const glm::vec3& p2Normal,
                                             const glm::vec3& p3,
                                             const glm::vec3& p3Color,
                                             const glm::vec3& p3Normal,
                                             const ResourceGroupID& resource)
{
  constexpr size_t POS3_SIZE = sizeof(glm::vec3);
  constexpr size_t COLOR3_SIZE = sizeof(glm::vec3);
  constexpr size_t NORM3_SIZE = sizeof(glm::vec3);

  auto& renderer = GetRenderer();
  renderer.DrawTriangle(
    {
      { &p1, POS3_SIZE },
      { &p1Color, COLOR3_SIZE },
      { &p1Normal, NORM3_SIZE },
      { &p2, POS3_SIZE },
      { &p2Color, COLOR3_SIZE },
      { &p2Normal, NORM3_SIZE },
      { &p3, POS3_SIZE },
      { &p3Color, COLOR3_SIZE },
      { &p3Normal, NORM3_SIZE },
    },
    resource);
}

// ----------------------------------------------------------------------------
