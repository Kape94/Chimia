#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

namespace ShaderCodes {

namespace Fragment {

inline const char* colored = R"(
    #version 330

    @embed(CUSTOM_UNIFORMS)

    @include "fragmentInputLayout::C3"

    out vec4 outputColor;

    void main() {
      outputColor = vec4(fragmentColor, 1.0);

      @embed(OUTPUT_OVERRIDERS)
    }
  )";

inline const char* gouraudLit = R"(
    #version 330

    @embed(CUSTOM_UNIFORMS)

    @include "fragmentInputLayout::C3"

    out vec4 outputColor;

    void main()
    {
      outputColor = vec4(fragmentColor, 1.0f);

      @embed(OUTPUT_OVERRIDERS)
    }
  )";

inline const char* phongLit = R"(
    #version 330
  
    @include "common::constants"
    @include "common::lightsTypes"
    @include "common::materialType"
    @include "common::calculateLights"
  
    uniform Material material;
  
    @include "common::lightsUniforms"
    @embed(CUSTOM_UNIFORMS)
  
    @include "fragmentInputLayout::P3N3"
  
    out vec4 outputColor;
  
    void main()
    {
        vec3 directional = CalculateDirectionalLight(
          viewPosition, 
          fragmentPos, 
          fragmentNorm, 
          nDirectionalLights, 
          directionalLights,
          material
        );
        vec3 point = CalculatePointLight(
          viewPosition, 
          fragmentPos, 
          fragmentNorm, 
          nPointLights, 
          pointLights,
          material
        );
      
        vec3 result = directional + point;
        outputColor = vec4(result, 1.0);

        @embed(OUTPUT_OVERRIDERS)
    }
    )";

inline const char* phongLitWithInstancedTransformAndMaterial = R"(
    #version 330
  
    @include "common::constants"
    @include "common::lightsTypes"
    @include "common::materialType"
    @include "common::calculateLights"
  
    uniform Material material;

    @include "common::lightsUniforms"
    @embed(CUSTOM_UNIFORMS)
  
    @include "fragmentInputLayout::P3N3"
  
    out vec4 outputColor;
  
    void main()
    {
        vec3 directional = CalculateDirectionalLight(
          viewPosition, 
          fragmentPos, 
          fragmentNorm, 
          nDirectionalLights, 
          directionalLights,
          material
        );
        vec3 point = CalculatePointLight(
          viewPosition, 
          fragmentPos, 
          fragmentNorm, 
          nPointLights, 
          pointLights,
          material
        );
      
        vec3 result = directional + point;
        outputColor = vec4(result, 1.0f);

        @embed(OUTPUT_OVERRIDERS)
    }
    )";

inline const char* phongLitByVertexColor = R"(
      #version 330
    
      @include "common::constants"
      @include "common::lightsTypes"
      @include "common::materialType"
      @include "common::calculateLightsWithoutMaterial"
    
      @include "common::lightsUniforms"
      @embed(CUSTOM_UNIFORMS)

      @include "fragmentInputLayout::P3C3N3"
    
      out vec4 outputColor;
    
      void main()
      {
          vec3 directional = CalculateDirectionalLight(
            viewPosition, 
            fragmentPos, 
            fragmentNorm, 
            nDirectionalLights, 
            directionalLights,
            fragmentColor
          );
          vec3 point = CalculatePointLight(
            viewPosition, 
            fragmentPos, 
            fragmentNorm, 
            nPointLights, 
            pointLights,
            fragmentColor
          );
        
          vec3 result = directional + point;
          outputColor = vec4(result, 1.0);

          @embed(OUTPUT_OVERRIDERS)
      }
      )";

inline const char* textured = R"(
      #version 330

      @include "fragmentInputLayout::T2"

      out vec4 outputColor;

      uniform sampler2D tex;
      @embed(CUSTOM_UNIFORMS)

      void main() {
        outputColor = texture(tex, fragmentTexCoord);

        @embed(OUTPUT_OVERRIDERS)
      }
    )";

inline const char* coloredTextured = R"(
      #version 330

      @include "fragmentInputLayout::C3T2"

      out vec4 outputColor;

      uniform sampler2D tex;
      @embed(CUSTOM_UNIFORMS)

      void main() {
        outputColor = vec4(fragmentColor * vec3(texture(tex, fragmentTexCoord)), 1.0);

        @embed(OUTPUT_OVERRIDERS)
      }
    )";

inline const char* gouraudLitTextured = R"(
      #version 330

      @include "fragmentInputLayout::T2Light"

      out vec4 outputColor;

      uniform sampler2D tex;
      @embed(CUSTOM_UNIFORMS)

      void main() {
        outputColor = vec4(fragmentLightColor, 1.0) * texture(tex, fragmentTexCoord);

        @embed(OUTPUT_OVERRIDERS)
      }
    )";

inline const char* phongLitTextured = R"(
      #version 330

      @include "common::constants"
      @include "common::lightsTypes"
      @include "common::materialType"
      @include "common::calculateLightsWithoutMaterial"
    
      @include "common::lightsUniforms"
      
      @include "fragmentInputLayout::P3N3T2"

      out vec4 outputColor;

      uniform sampler2D tex;
      @embed(CUSTOM_UNIFORMS)

      void main() {
        vec3 fragmentColor = vec3(texture(tex, fragmentTexCoord));
        
        vec3 directional = CalculateDirectionalLight(
            viewPosition, 
            fragmentPos, 
            fragmentNorm, 
            nDirectionalLights, 
            directionalLights,
            fragmentColor
          );
          vec3 point = CalculatePointLight(
            viewPosition, 
            fragmentPos, 
            fragmentNorm, 
            nPointLights, 
            pointLights,
            fragmentColor
          );
        
          vec3 result = directional + point;
          outputColor = vec4(result, 1.0);

          @embed(OUTPUT_OVERRIDERS)
      }
    )";

inline const char* phongLitColoredTextured = R"(
      #version 330

      @include "common::constants"
      @include "common::lightsTypes"
      @include "common::materialType"
      @include "common::calculateLightsWithoutMaterial"
    
      @include "common::lightsUniforms"
      
      @include "fragmentInputLayout::P3C3N3T2"

      out vec4 outputColor;

      uniform sampler2D tex;
      @embed(CUSTOM_UNIFORMS)

      void main() {
        vec3 color = vec3(texture(tex, fragmentTexCoord)) * fragmentColor;
        
        vec3 directional = CalculateDirectionalLight(
            viewPosition, 
            fragmentPos, 
            fragmentNorm, 
            nDirectionalLights, 
            directionalLights,
            color
          );
          vec3 point = CalculatePointLight(
            viewPosition, 
            fragmentPos, 
            fragmentNorm, 
            nPointLights, 
            pointLights,
            color
          );
        
          vec3 result = directional + point;
          outputColor = vec4(result, 1.0);

          @embed(OUTPUT_OVERRIDERS)
      }
    )";
}

}

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------