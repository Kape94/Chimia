#include "RenderersUtils.h"

#include "Config.h"
#include "Pipelines.h"
#include "ResourceGroup.h"
#include "ResourcesManager.h"
#include "Types.h"

#include "Core/Diagnostics.h"
#include "Rendering/Shader.h"
#include "Rendering/ShaderAttribute.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
// RenderersUtilsPrivate
// ----------------------------------------------------------------------------

namespace RenderersUtilsPrivate {
using namespace Chimia;

namespace Uniforms {
const std::string OPACITY = "u_opacity";
const std::string MIXTURE_COLOR = "u_mixtureColor";

const std::string HAS_VERTEX_COLOR = "u_hasVertexColor";
const std::string HAS_NORMAL = "u_hasNormal";
const std::string HAS_TEXCOORD = "u_hasTexCoord";
const std::string IS_INSTANCED = "u_isInstanced";
const std::string HAS_MATERIAL = "u_hasMaterial";
const std::string HAS_TEXTURE = "u_hasTexture";
const std::string LIGHTNING_MODEL = "u_lightningModel";
}

Rendering::ShaderAttribute
PositionAttribute()
{
  return Rendering::ShaderAttribute::Float(0 /*pos*/, 3);
}

Rendering::ShaderAttribute
ColorAttribute()
{
  return Rendering::ShaderAttribute::Float(1 /*color*/, 4);
}

Rendering::ShaderAttribute
NormalAttribute()
{
  return Rendering::ShaderAttribute::Float(2 /*normal*/, 3);
}

Rendering::ShaderAttribute
TexCoordAttribute()
{
  return Rendering::ShaderAttribute::Float(3 /*texCoord*/, 2);
}

Rendering::ShaderAttributes
TransformAttributes()
{
  return { Rendering::ShaderAttribute::Float(4 /*transform*/, 4),
           Rendering::ShaderAttribute::Float(5 /*transform*/, 4),
           Rendering::ShaderAttribute::Float(6 /*transform*/, 4),
           Rendering::ShaderAttribute::Float(7 /*transform*/, 4) };
}

bool
HasColor(const eVertexLayout& layout)
{
  return layout == eVertexLayout::POSITION3_COLOR4 ||
         layout == eVertexLayout::POSITION3_COLOR4_NORMAL3 ||
         layout == eVertexLayout::POSITION3_COLOR4_TEXCOORD2 ||
         layout == eVertexLayout::POSITION3_COLOR4_NORMAL3_TEXCOORD2;
}

bool
HasNormal(const eVertexLayout& layout)
{
  return layout == eVertexLayout::POSITION3_NORMAL3 ||
         layout == eVertexLayout::POSITION3_COLOR4_NORMAL3 ||
         layout == eVertexLayout::POSITION3_NORMAL3_TEXCOORD2 ||
         layout == eVertexLayout::POSITION3_COLOR4_NORMAL3_TEXCOORD2;
}

bool
HasTexCoord(const eVertexLayout& layout)
{
  return layout == eVertexLayout::POSITION3_TEXCOORD2 ||
         layout == eVertexLayout::POSITION3_COLOR4_TEXCOORD2 ||
         layout == eVertexLayout::POSITION3_NORMAL3_TEXCOORD2 ||
         layout == eVertexLayout::POSITION3_COLOR4_NORMAL3_TEXCOORD2;
}

void
ConfigureOpacity(Rendering::Shader& shader,
                 const eVertexLayout& layout,
                 const ResourcesGroup& resources)
{
  const bool allowsOpacity = !HasNormal(layout) && resources.HasOpacityFactor();

  if (allowsOpacity) {
    const OpacityFactorID opacityID = resources.FirstOpacityFactor();
    const float* opacity =
      ResourcesManager::GetInstance().GetOpacityFactor(opacityID);

    shader.SetUniform(Uniforms::OPACITY, *opacity);
  } else {
    shader.SetUniform(Uniforms::OPACITY, 1.0f);
  }
}

void
ConfigureMixtureColor(Rendering::Shader& shader,
                      const ResourcesGroup& resources)
{
  if (resources.HasMixtureColor()) {
    const MixtureColorID colorID = resources.FirstMixtureColor();
    const glm::vec3* color =
      ResourcesManager::GetInstance().GetMixtureColor(colorID);

    shader.SetUniform(Uniforms::MIXTURE_COLOR, *color);

  } else {
    shader.SetUniform(Uniforms::MIXTURE_COLOR, glm::vec3(1.0f, 1.0f, 1.0f));
  }
}

void
ConfigureShaderForRendering(Rendering::Shader& shader,
                            const eVertexLayout& layout,
                            const bool isInstancedRendering,
                            const ResourcesGroup& resources)
{
  shader.SetUniform(Uniforms::HAS_VERTEX_COLOR, HasColor(layout));
  shader.SetUniform(Uniforms::HAS_NORMAL, HasNormal(layout));
  shader.SetUniform(Uniforms::HAS_TEXCOORD, HasTexCoord(layout));
  shader.SetUniform(Uniforms::IS_INSTANCED, isInstancedRendering);
  shader.SetUniform(Uniforms::HAS_MATERIAL, resources.HasMaterials());
  shader.SetUniform(Uniforms::HAS_TEXTURE, resources.HasTextures());

  Pipelines::CurrentPipeline().ConfigureShader(shader);

  const int illuminationModel = static_cast<int>(Config::IlluminationModel());
  shader.SetUniform(Uniforms::LIGHTNING_MODEL, illuminationModel);

  ConfigureOpacity(shader, layout, resources);
  ConfigureMixtureColor(shader, resources);
}
}

// ----------------------------------------------------------------------------
// RenderersUtils
// ----------------------------------------------------------------------------

VertexLayoutAttributes
RenderersUtils::GetAttributesForLayout(const eVertexLayout& layout)
{
  using namespace RenderersUtilsPrivate;

  switch (layout) {
    case eVertexLayout::POSITION3_COLOR4: {
      return { { PositionAttribute(), ColorAttribute() },
               TransformAttributes() };
    }
    case eVertexLayout::POSITION3_NORMAL3: {
      return { { PositionAttribute(), NormalAttribute() },
               TransformAttributes() };
    }
    case eVertexLayout::POSITION3_TEXCOORD2: {
      return { { PositionAttribute(), TexCoordAttribute() },
               TransformAttributes() };
    }
    case eVertexLayout::POSITION3_COLOR4_NORMAL3: {
      return { { PositionAttribute(), ColorAttribute(), NormalAttribute() },
               TransformAttributes() };
    }
    case eVertexLayout::POSITION3_COLOR4_TEXCOORD2: {
      return { { PositionAttribute(), ColorAttribute(), TexCoordAttribute() },
               TransformAttributes() };
    }
    case eVertexLayout::POSITION3_NORMAL3_TEXCOORD2: {
      return { { PositionAttribute(), NormalAttribute(), TexCoordAttribute() },
               TransformAttributes() };
    }
    case eVertexLayout::POSITION3_COLOR4_NORMAL3_TEXCOORD2: {
      return { { PositionAttribute(),
                 ColorAttribute(),
                 NormalAttribute(),
                 TexCoordAttribute() },
               TransformAttributes() };
    }
    case eVertexLayout::UNDEFINED:
    default: {
      Diagnostics::Error(1, "Unable to fetch attributes for undefined layout");
      return {};
    }
  }
}

// ----------------------------------------------------------------------------

void
RenderersUtils::ConfigureShaderForRendering(Rendering::Shader& shader,
                                            const eVertexLayout& layout,
                                            const ResourcesGroup& resources)
{
  RenderersUtilsPrivate::ConfigureShaderForRendering(
    shader, layout, false /*isInstancedRendering*/, resources);
}

// ----------------------------------------------------------------------------

void
RenderersUtils::ConfigureShaderForInstancedRendering(
  Rendering::Shader& shader,
  const eVertexLayout& layout,
  const ResourcesGroup& resources)
{
  RenderersUtilsPrivate::ConfigureShaderForRendering(
    shader, layout, true /*isInstancedRendering*/, resources);
}

// ----------------------------------------------------------------------------