#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

namespace ShaderCodes {

namespace Vertex {

inline const char* generic = R"(
  #version 330

  @include "common::constants"
  @include "common::lightsTypes"
  @include "common::materialType"
  @include "common::calculateLights"
  @include "common::calculateLightsWithoutMaterial"

  @include "common::lightsUniforms"
  uniform mat4 u_cameraTransform;
  @embed(CUSTOM_UNIFORMS)

  uniform Material u_material;
  
  uniform bool u_hasVertexColor;
  uniform bool u_hasNormal;
  uniform bool u_hasTexCoord;
  uniform bool u_isInstanced;
  uniform bool u_hasMaterial;
  uniform bool u_hasTexture;

  uniform bool u_isTransparentRendering;

  uniform int u_lightningModel;

  layout (location = 0) in vec3 a_vertexPos;
  layout (location = 1) in vec4 a_vertexColor;
  layout (location = 2) in vec3 a_vertexNorm;
  layout (location = 3) in vec2 a_vertexTexCoord;
  layout (location = 4) in mat4 a_instanceTransform;

  out vec3 a_fragmentPos;
  out vec4 a_fragmentColor;
  out vec3 a_fragmentNorm;
  out vec2 a_fragmentTexCoord;

  void main() {
    vec4 transformedPos = u_isInstanced ? a_instanceTransform * vec4(a_vertexPos, 1.0) : vec4(a_vertexPos, 1.0);

    vec3 pos = vec3(transformedPos);
    vec3 norm = u_isInstanced ? mat3(transpose(inverse(a_instanceTransform))) * a_vertexNorm : a_vertexNorm;

    vec3 directional = vec3(0.0, 0.0, 0.0);
    vec3 point = vec3(0.0, 0.0, 0.0);

    bool shouldCalculateLights = u_lightningModel == 0/*gouraud*/ && u_hasNormal && !u_isTransparentRendering;
    if (shouldCalculateLights)
    {
      if (u_hasMaterial)
      {
        directional = CalculateDirectionalLight(
          u_viewPosition, 
          pos, 
          norm, 
          u_nDirectionalLights, 
          u_directionalLights,
          u_material
        );
        point = CalculatePointLight(
          u_viewPosition, 
          pos, 
          norm, 
          u_nPointLights, 
          u_pointLights,
          u_material
        );
      }
      else
      {
        vec3 neutralColor = vec3(1.0, 1.0, 1.0);
        directional = CalculateDirectionalLight(
          u_viewPosition, 
          pos, 
          norm, 
          u_nDirectionalLights, 
          u_directionalLights,
          neutralColor
        );
        point = CalculatePointLight(
          u_viewPosition, 
          pos, 
          norm, 
          u_nPointLights, 
          u_pointLights,
          neutralColor
        );
      }
    }

    vec4 result = vec4(directional + point, 1.0f);

    a_fragmentPos = pos;
    a_fragmentNorm = norm;

    if (shouldCalculateLights)
    {
      a_fragmentColor = u_hasMaterial ? result : result * a_vertexColor;
    }
    else 
    {
      a_fragmentColor = u_hasVertexColor ? a_vertexColor : vec4(1.0, 1.0, 1.0, 1.0);
    }
    a_fragmentTexCoord = a_vertexTexCoord;

    gl_Position = u_cameraTransform * transformedPos;

    @embed(OUTPUT_OVERRIDERS)
  }
)";

}

}

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------