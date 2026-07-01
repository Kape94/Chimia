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
  uniform mat4 cameraTransform;
  @embed(CUSTOM_UNIFORMS)

  uniform Material material;
  
  uniform bool hasVertexColor;
  uniform bool hasNormal;
  uniform bool hasTexCoord;
  uniform bool isInstanced;
  uniform bool hasMaterial;
  uniform bool hasTexture;

  uniform bool isTransparentRendering;

  uniform int lightningModel;

  layout (location = 0) in vec3 vertexPos;
  layout (location = 1) in vec4 vertexColor;
  layout (location = 2) in vec3 vertexNorm;
  layout (location = 3) in vec2 vertexTexCoord;
  layout (location = 4) in mat4 instanceTransform;

  out vec3 fragmentPos;
  out vec4 fragmentColor;
  out vec3 fragmentNorm;
  out vec2 fragmentTexCoord;

  void main() {
    vec4 transformedPos = isInstanced ? instanceTransform * vec4(vertexPos, 1.0) : vec4(vertexPos, 1.0);

    vec3 pos = vec3(transformedPos);
    vec3 norm = isInstanced ? mat3(transpose(inverse(instanceTransform))) * vertexNorm : vertexNorm;

    vec3 directional = vec3(0.0, 0.0, 0.0);
    vec3 point = vec3(0.0, 0.0, 0.0);

    bool shouldCalculateLights = lightningModel == 0/*gouraud*/ && hasNormal && !isTransparentRendering;
    if (shouldCalculateLights)
    {
      if (hasMaterial)
      {
        directional = CalculateDirectionalLight(
          viewPosition, 
          pos, 
          norm, 
          nDirectionalLights, 
          directionalLights,
          material
        );
        point = CalculatePointLight(
          viewPosition, 
          pos, 
          norm, 
          nPointLights, 
          pointLights,
          material
        );
      }
      else
      {
        vec3 neutralColor = vec3(1.0, 1.0, 1.0);
        directional = CalculateDirectionalLight(
          viewPosition, 
          pos, 
          norm, 
          nDirectionalLights, 
          directionalLights,
          neutralColor
        );
        point = CalculatePointLight(
          viewPosition, 
          pos, 
          norm, 
          nPointLights, 
          pointLights,
          neutralColor
        );
      }
    }

    vec4 result = vec4(directional + point, 1.0f);

    fragmentPos = pos;
    fragmentNorm = norm;

    if (shouldCalculateLights)
    {
      fragmentColor = hasMaterial ? result : result * vertexColor;
    }
    else 
    {
      fragmentColor = hasVertexColor ? vertexColor : vec4(1.0, 1.0, 1.0, 1.0);
    }
    fragmentTexCoord = vertexTexCoord;

    gl_Position = cameraTransform * transformedPos;

    @embed(OUTPUT_OVERRIDERS)
  }
)";

}

}

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------