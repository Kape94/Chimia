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

      uniform bool isTransparentRendering;

      uniform int lightningModel;
      uniform float opacity;
      uniform vec3 mixtureColor;

      in vec3 fragmentPos;
      in vec4 fragmentColor;
      in vec3 fragmentNorm;
      in vec2 fragmentTexCoord;

      out vec4 outputColor;

      uniform sampler2D tex;
      @embed(CUSTOM_UNIFORMS)

      void main() {
        vec4 color = fragmentColor;
        if (hasTexture && hasTexCoord)
        {
          color = texture(tex, fragmentTexCoord) * color;
        }

        float colorOpacity = opacity * color.a;
        if (!isTransparentRendering)
        {
          if (colorOpacity < 0.9)
          {
            discard;
          }
        }
        else /*is transparent rendering*/
        {
          // discard opaque and cutout
          if (colorOpacity >= 0.9 || colorOpacity < 0.1)
          {
            discard;
          }
        }

        vec3 directional = vec3(0.0, 0.0, 0.0);
        vec3 point = vec3(0.0, 0.0, 0.0);

        bool shouldCalculateLights = lightningModel == 1/*phong*/ && hasNormal && !isTransparentRendering;
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
              vec3 neutralColor = vec3(1.0, 1.0, 1.0);
              directional = CalculateDirectionalLight(
                viewPosition, 
                fragmentPos, 
                fragmentNorm, 
                nDirectionalLights, 
                directionalLights,
                neutralColor
              );
              point = CalculatePointLight(
                viewPosition, 
                fragmentPos, 
                fragmentNorm, 
                nPointLights, 
                pointLights,
                neutralColor
              );
            }
        }

        if (shouldCalculateLights)
        {
          vec4 shadedColor = vec4(directional + point, 1.0);
          outputColor = hasMaterial ? shadedColor : shadedColor * color;
        }
        else 
        {
          outputColor = color * vec4(1.0, 1.0, 1.0, opacity);
        }
        
        outputColor *= vec4(mixtureColor, 1.0);
        @embed(OUTPUT_OVERRIDERS)
      }
    )";
}

}

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------