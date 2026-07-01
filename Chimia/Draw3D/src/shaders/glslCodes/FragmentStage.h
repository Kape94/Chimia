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
      
      uniform Material u_material;
  
      uniform bool u_hasVertexColor;
      uniform bool u_hasNormal;
      uniform bool u_hasTexCoord;
      uniform bool u_isInstanced;
      uniform bool u_hasMaterial;
      uniform bool u_hasTexture;

      uniform bool u_isTransparentRendering;

      uniform int u_lightningModel;
      uniform float u_opacity;
      uniform vec3 u_mixtureColor;

      in vec3 a_fragmentPos;
      in vec4 a_fragmentColor;
      in vec3 a_fragmentNorm;
      in vec2 a_fragmentTexCoord;

      out vec4 out_color;

      uniform sampler2D u_tex;
      @embed(CUSTOM_UNIFORMS)

      void main() {
        vec4 color = a_fragmentColor;
        if (u_hasTexture && u_hasTexCoord)
        {
          color = texture(u_tex, a_fragmentTexCoord) * color;
        }

        float colorOpacity = u_opacity * color.a;
        if (!u_isTransparentRendering)
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

        bool shouldCalculateLights = u_lightningModel == 1/*phong*/ && u_hasNormal && !u_isTransparentRendering;
        if (shouldCalculateLights)
        {
            if (u_hasMaterial)
            {
              directional = CalculateDirectionalLight(
                u_viewPosition, 
                a_fragmentPos, 
                a_fragmentNorm, 
                u_nDirectionalLights, 
                u_directionalLights,
                u_material
              );
              point = CalculatePointLight(
                u_viewPosition, 
                a_fragmentPos, 
                a_fragmentNorm, 
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
                a_fragmentPos, 
                a_fragmentNorm, 
                u_nDirectionalLights, 
                u_directionalLights,
                neutralColor
              );
              point = CalculatePointLight(
                u_viewPosition, 
                a_fragmentPos, 
                a_fragmentNorm, 
                u_nPointLights, 
                u_pointLights,
                neutralColor
              );
            }
        }

        if (shouldCalculateLights)
        {
          vec4 shadedColor = vec4(directional + point, 1.0);
          out_color = u_hasMaterial ? shadedColor : shadedColor * color;
        }
        else 
        {
          out_color = color * vec4(1.0, 1.0, 1.0, u_opacity);
        }
        
        out_color *= vec4(u_mixtureColor, 1.0);
        @embed(OUTPUT_OVERRIDERS)
      }
    )";
}

}

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------