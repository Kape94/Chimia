#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

namespace ShaderCodes {

namespace Fragment {

inline const char* generic = R"(
      #version 330

      @include "common::constants"
      @include "common::lightsTypes"
      @include "common::materialType"
      @include "common::calculateLights"
      @include "common::calculateLightsWithoutMaterial"
    
      @include "common::lightsUniforms"
      
      uniform Material material;
  
      uniform bool hasVertexColor;
      uniform bool hasNormal;
      uniform bool hasTexCoord;
      uniform bool isInstanced;
      uniform bool hasMaterial;
      uniform bool hasTexture;

      uniform int lightningModel;

      in vec3 fragmentPos;
      in vec3 fragmentColor;
      in vec3 fragmentNorm;
      in vec2 fragmentTexCoord;

      out vec4 outputColor;

      uniform sampler2D tex;
      @embed(CUSTOM_UNIFORMS)

      void main() {
        vec3 color = fragmentColor;
        if (hasTexture && hasTexCoord)
        {
          color = vec3(texture(tex, fragmentTexCoord)) * color;
        }        

        vec3 directional = vec3(0.0, 0.0, 0.0);
        vec3 point = vec3(0.0, 0.0, 0.0);

        bool shouldCalculateLights = lightningModel == 1/*phong*/ && hasNormal;
        if (shouldCalculateLights)
        {
            if (hasMaterial)
            {
              directional = CalculateDirectionalLight(
                viewPosition, 
                fragmentPos, 
                fragmentNorm, 
                nDirectionalLights, 
                directionalLights,
                material
              );
              point = CalculatePointLight(
                viewPosition, 
                fragmentPos, 
                fragmentNorm, 
                nPointLights, 
                pointLights,
                material
              );
            }
            else
            {
              directional = CalculateDirectionalLight(
                viewPosition, 
                fragmentPos, 
                fragmentNorm, 
                nDirectionalLights, 
                directionalLights,
                color
              );
              point = CalculatePointLight(
                viewPosition, 
                fragmentPos, 
                fragmentNorm, 
                nPointLights, 
                pointLights,
                color
              );
            }
        }

        outputColor = shouldCalculateLights ? vec4(directional + point, 1.0) : vec4(color, 1.0);
        
          @embed(OUTPUT_OVERRIDERS)
      }
    )";
}

}

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------