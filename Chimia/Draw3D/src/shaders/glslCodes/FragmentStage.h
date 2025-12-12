#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

namespace ShaderCodes {

namespace Fragment {

inline const char* colored = R"(
    #version 330

    in vec3 vertexColor;

    out vec4 fragColor;

    void main() {
      fragColor = vec4(vertexColor, 1.0);
    }
  )";

inline const char* gouraudLit = R"(
    #version 330

    in vec3 color;

    out vec4 fragColor;

    void main()
    {
      fragColor = vec4(color, 1.0f);
    }
  )";

inline const char* phongLit = R"(
    #version 330
  
    @include "common::constants"
    @include "common::lightsTypes"
    @include "common::materialType"
    @include "common::calculateLights"
  
    uniform Material material;
  
    uniform DirectionalLight directionalLights[MAX_LIGHTS];
    uniform int nDirectionalLights;
  
    uniform PointLight pointLights[MAX_LIGHTS];
    uniform int nPointLights;
  
    uniform vec3 viewPosition;
  
    in vec3 fragmentPos;
    in vec3 fragmentNorm;
  
    out vec4 fragColor;
  
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
        fragColor = vec4(result, 1.0);
    }
    )";

inline const char* phongLitWithInstancedTransformAndMaterial = R"(
    #version 330
  
    @include "common::constants"
    @include "common::lightsTypes"
    @include "common::materialType"
    @include "common::calculateLights"
  
    uniform DirectionalLight directionalLights[MAX_LIGHTS];
    uniform int nDirectionalLights;
  
    uniform PointLight pointLights[MAX_LIGHTS];
    uniform int nPointLights;
  
    uniform vec3 viewPosition;
  
    in vec3 fragmentPos;
    in vec3 fragmentNorm;
    in vec3 mAmbient;
    in vec3 mDiffuse;
    in vec3 mSpecular;
    in float mShininess;
  
    out vec4 fragColor;
  
    void main()
    {
        vec3 directional = CalculateDirectionalLight(
          viewPosition, 
          fragmentPos, 
          fragmentNorm, 
          nDirectionalLights, 
          directionalLights,
          mAmbient,
          mDiffuse,
          mSpecular,
          mShininess
        );
        vec3 point = CalculatePointLight(
          viewPosition, 
          fragmentPos, 
          fragmentNorm, 
          nPointLights, 
          pointLights,
          mAmbient,
          mDiffuse,
          mSpecular,
          mShininess
        );
      
        vec3 result = directional + point;
        fragColor = vec4(result, 1.0f);
    }
    )";

inline const char* phongLitByVertexColor = R"(
      #version 330
    
      @include "common::constants"
      @include "common::lightsTypes"
      @include "common::materialType"
      @include "common::calculateLightsWithoutMaterial"
    
      uniform DirectionalLight directionalLights[MAX_LIGHTS];
      uniform int nDirectionalLights;
    
      uniform PointLight pointLights[MAX_LIGHTS];
      uniform int nPointLights;
    
      uniform vec3 viewPosition;
    
      in vec3 fragmentPos;
      in vec3 fragmentColor;
      in vec3 fragmentNorm;
    
      out vec4 fragColor;
    
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
          fragColor = vec4(result, 1.0);
      }
      )";

inline const char* textured = R"(
      #version 330

      in vec2 fragUV;

      out vec4 fragColor;

      uniform sampler2D tex;

      void main() {
        fragColor = texture(tex, fragUV);
      }
    )";

inline const char* coloredTextured = R"(
      #version 330

      in vec3 fragmentColor;
      in vec2 fragUV;

      out vec4 fragColor;

      uniform sampler2D tex;

      void main() {
        fragColor = vec4(fragmentColor * vec3(texture(tex, fragUV)), 1.0);
      }
    )";

inline const char* gouraudLitTextured = R"(
      #version 330

      in vec2 fragmentTexCoord;
      in vec3 fragmentLightColor;

      out vec4 fragColor;

      uniform sampler2D tex;

      void main() {
        fragColor = vec4(fragmentLightColor, 1.0) * texture(tex, fragmentTexCoord);
      }
    )";

inline const char* phongLitTextured = R"(
      #version 330

      @include "common::constants"
      @include "common::lightsTypes"
      @include "common::materialType"
      @include "common::calculateLightsWithoutMaterial"
    
      uniform DirectionalLight directionalLights[MAX_LIGHTS];
      uniform int nDirectionalLights;
    
      uniform PointLight pointLights[MAX_LIGHTS];
      uniform int nPointLights;
    
      uniform vec3 viewPosition;
      
      in vec3 fragmentPos;
      in vec2 fragmentTexCoord;
      in vec3 fragmentNorm;

      out vec4 fragColor;

      uniform sampler2D tex;

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
          fragColor = vec4(result, 1.0);
      }
    )";

inline const char* phongLitColoredTextured = R"(
      #version 330

      @include "common::constants"
      @include "common::lightsTypes"
      @include "common::materialType"
      @include "common::calculateLightsWithoutMaterial"
    
      uniform DirectionalLight directionalLights[MAX_LIGHTS];
      uniform int nDirectionalLights;
    
      uniform PointLight pointLights[MAX_LIGHTS];
      uniform int nPointLights;
    
      uniform vec3 viewPosition;
      
      in vec3 fragmentPos;
      in vec3 fragmentColor;
      in vec2 fragmentTexCoord;
      in vec3 fragmentNorm;

      out vec4 fragColor;

      uniform sampler2D tex;

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
          fragColor = vec4(result, 1.0);
      }
    )";
}

}

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------