#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

namespace ShaderCodes {

namespace Common {

inline const char* constants = R"(
  #define MAX_LIGHTS 4

)";

inline const char* materialType = R"(
    struct Material {
      vec3 ambient;
      vec3 diffuse;
      vec3 specular;
      float shininess;
    };
)";

inline const char* lightsTypes = R"(
  struct DirectionalLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 direction;
  };

  struct PointLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
  
    vec3 position;
    float constant;
    float linear;
    float quadratic;
  };
)";

inline const char* calculateLights = R"(
      @include "common::constants"

      vec3 CalculateDirectionalLight(
        vec3 viewPosition, 
        vec3 fragmentPos, 
        vec3 fragmentNorm, 
        int nDirectionalLights, 
        DirectionalLight directionalLights[MAX_LIGHTS],
        vec3 mAmbient,
        vec3 mDiffuse,
        vec3 mSpecular,
        float mShininess
    ) 
    {
        vec3 viewDir = normalize(viewPosition - fragmentPos);
      
        vec3 normal = normalize(fragmentNorm);
  
        vec3 result = vec3(0.0);
        int nLights = min(nDirectionalLights, MAX_LIGHTS);
        for (int i = 0; i < nLights; ++i) {
            DirectionalLight light = directionalLights[i];
          
            vec3 lightDir = normalize(light.direction);
            vec3 invLightDir = -lightDir;
          
            float diff = max(dot(normal, invLightDir), 0.0);
          
            vec3 reflectDir = reflect(lightDir, normal);
            float spec = pow(max(dot(reflectDir, viewDir), 0.0), mShininess);
          
            vec3 ambient = light.ambient * mAmbient;
            vec3 diffuse = light.diffuse * (diff * mDiffuse);
            vec3 specular = light.specular * (spec * mSpecular);
          
            result += (ambient + diffuse + specular);
        }
        return result; 
    }

    vec3 CalculateDirectionalLight(
        vec3 viewPosition, 
        vec3 fragmentPos, 
        vec3 fragmentNorm, 
        int nDirectionalLights, 
        DirectionalLight directionalLights[MAX_LIGHTS],
        Material material
    ) 
    {
        return CalculateDirectionalLight(
            viewPosition, 
            fragmentPos, 
            fragmentNorm, 
            nDirectionalLights, 
            directionalLights,
            material.ambient,
            material.diffuse,
            material.specular,
            material.shininess
        );
    }
  
    vec3 CalculatePointLight(
        vec3 viewPosition, 
        vec3 fragmentPos, 
        vec3 fragmentNorm, 
        int nPointLights, 
        PointLight pointLights[MAX_LIGHTS],
        vec3 mAmbient,
        vec3 mDiffuse,
        vec3 mSpecular,
        float mShininess
    ) 
    {
        vec3 viewDir = normalize(viewPosition - fragmentPos);
        vec3 normal = normalize(fragmentNorm);
  
        vec3 result = vec3(0.0);
        int nLights = min(nPointLights, MAX_LIGHTS);
        for (int i = 0; i < nLights; ++i) {
            PointLight light = pointLights[i];
          
            vec3 lightDir = normalize(fragmentPos - light.position);
            vec3 invLightDir = -lightDir;
          
            float diff = max(dot(invLightDir, normal), 0.0);
          
            vec3 reflectDir = reflect(lightDir, normal);
            float spec = pow( max(dot(reflectDir, viewDir), 0.0) , mShininess );
          
            float distance = length(light.position - fragmentPos);
            float factor = light.constant + light.linear * distance + light.quadratic * (distance * distance);
            float attenuation = 1.0 / factor;
          
            vec3 ambient = (light.ambient * mAmbient) * attenuation;
            vec3 diffuse = ( light.diffuse * (diff * mDiffuse) ) * attenuation;
            vec3 specular = ( light.specular * (spec * mSpecular) ) * attenuation;
          
            result += (ambient + diffuse + specular);
          
        }
        return result;
    }

    vec3 CalculatePointLight(
      vec3 viewPosition, 
      vec3 fragmentPos, 
      vec3 fragmentNorm, 
      int nPointLights, 
      PointLight pointLights[MAX_LIGHTS],
      Material material
    ) 
    {
    	return CalculatePointLight(
            viewPosition, 
            fragmentPos, 
            fragmentNorm, 
            nPointLights, 
        	pointLights,
            material.ambient,
            material.diffuse,
            material.specular,
            material.shininess
        );
	  }
)";

}

namespace Vertex {

inline const char* colored = R"(
    #version 330

    layout (location = 0) in vec3 pos;
    layout (location = 1) in vec3 color;

    out vec3 vertexColor;

    uniform mat4 cameraTransform;

    void main() {
      gl_Position = cameraTransform * vec4(pos, 1.0);
      vertexColor = color;
    }
  )";

inline const char* coloredWithInstancedTransform = R"(
    #version 330

    layout (location = 0) in vec3 pos;
    layout (location = 1) in vec3 color;
    layout (location = 2) in mat4 modelTransform;

    out vec3 vertexColor;

    uniform mat4 cameraTransform;

    void main() {
      gl_Position = cameraTransform * modelTransform * vec4(pos, 1.0);
      vertexColor = color;
    }
  )";

inline const char* gouraudLit = R"(
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

  uniform mat4 cameraTransform;

  layout (location = 0) in vec3 vertexPos;
  layout (location = 1) in vec3 vertexNorm;

  out vec3 color;

  void main()
  {
      vec3 directional = CalculateDirectionalLight(
        viewPosition, 
        vertexPos, 
        vertexNorm, 
        nDirectionalLights, 
        directionalLights,
        material
      );
      vec3 point = CalculatePointLight(
        viewPosition, 
        vertexPos, 
        vertexNorm, 
        nPointLights, 
        pointLights,
        material
      );
    
      vec3 result = directional + point;
	    color = result;

      gl_Position = cameraTransform * vec4(vertexPos, 1.0);
  }
  )";

inline const char* gouraudLitWithInstancedTransformAndMaterial = R"(
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
  
    uniform mat4 cameraTransform;
  
    layout (location = 0) in vec3 vertexPos;
    layout (location = 1) in vec3 vertexNorm;
    layout (location = 2) in mat4 modelTransform;
    layout (location = 6) in vec3 materialAmbient;
    layout (location = 7) in vec3 materialDiffuse;
    layout (location = 8) in vec3 materialSpecular;
    layout (location = 9) in float materialShininess;
  
    out vec3 color;
  
    void main()
    {
        vec3 directional = CalculateDirectionalLight(
          viewPosition, 
          vertexPos, 
          vertexNorm, 
          nDirectionalLights, 
          directionalLights,
          materialAmbient,
          materialDiffuse,
          materialSpecular,
          materialShininess
        );
        vec3 point = CalculatePointLight(
          viewPosition, 
          vertexPos, 
          vertexNorm, 
          nPointLights, 
          pointLights,
          materialAmbient,
          materialDiffuse,
          materialSpecular,
          materialShininess
        );
      
        vec3 result = directional + point;
        color = result;
  
        gl_Position = cameraTransform * modelTransform * vec4(vertexPos, 1.0);
    }
    )";

inline const char* phongLit = R"(
      #version 330
    
      layout (location = 0) in vec3 vertexPos;
      layout (location = 1) in vec3 vertexNorm;

      out vec3 fragmentPos;
      out vec3 fragmentNorm;
      
      uniform mat4 cameraTransform;

      void main()
      {
          fragmentPos = vertexPos;
          fragmentNorm = vertexNorm;
          
          gl_Position = cameraTransform * vec4(vertexPos, 1.0);
      }
      )";

inline const char* phongLitWithInstancedTransformAndMaterial = R"(
      #version 330
    
      layout (location = 0) in vec3 vertexPos;
      layout (location = 1) in vec3 vertexNorm;
      layout (location = 2) in mat4 modelTransform;
      layout (location = 6) in vec3 materialAmbient;
      layout (location = 7) in vec3 materialDiffuse;
      layout (location = 8) in vec3 materialSpecular;
      layout (location = 9) in float materialShininess;
    
      out vec3 fragmentPos;
      out vec3 fragmentNorm;
      out vec3 mAmbient;
      out vec3 mDiffuse;
      out vec3 mSpecular;
      out float mShininess;

      uniform mat4 cameraTransform;

      void main()
      {
          fragmentPos = vertexPos;
          fragmentNorm = vertexNorm;
          mAmbient = materialAmbient;
          mDiffuse = materialDiffuse;
          mSpecular = materialSpecular;
          mShininess = materialShininess;

          gl_Position = cameraTransform * modelTransform * vec4(vertexPos, 1.0);
      }
      )";

inline const char* gouraudLitWithVertexColor = R"(
        #version 330
      
        @include "common::constants"
        @include "common::lightsTypes"
      
        uniform DirectionalLight directionalLights[MAX_LIGHTS];
        uniform int nDirectionalLights;
      
        uniform PointLight pointLights[MAX_LIGHTS];
        uniform int nPointLights;
      
        uniform vec3 viewPosition;
      
        uniform mat4 cameraTransform;
      
        layout (location = 0) in vec3 vertexPos;
        layout (location = 1) in vec3 vertexColor;
        layout (location = 2) in vec3 vertexNorm;
      
        out vec3 color;
      
        vec3 CalculateDirectionalLight();
        vec3 CalculatePointLight();
      
        void main()
        {
            vec3 directional = CalculateDirectionalLight();
            vec3 point = CalculatePointLight();
          
            vec3 result = directional + point;
            color = result;
      
            gl_Position = cameraTransform * vec4(vertexPos, 1.0);
        }
      
      
        vec3 CalculateDirectionalLight() {
            vec3 viewDir = normalize(viewPosition - vertexPos);
          
            vec3 normal = normalize(vertexNorm);
      
            vec3 result = vec3(0.0);
            int nLights = min(nDirectionalLights, MAX_LIGHTS);
            for (int i = 0; i < nLights; ++i) {
                DirectionalLight light = directionalLights[i];
              
                vec3 lightDir = normalize(light.direction);
                vec3 invLightDir = -lightDir;
              
                float diff = max(dot(normal, invLightDir), 0.0);
              
                vec3 ambient = light.ambient;
                vec3 diffuse = light.diffuse * diff;
              
                result += (ambient + diffuse);
            }
            return result * vertexColor; 
        }
      
        vec3 CalculatePointLight() {
            vec3 viewDir = normalize(viewPosition - vertexPos);
            vec3 normal = normalize(vertexNorm);
      
            vec3 result = vec3(0.0);
            int nLights = min(nPointLights, MAX_LIGHTS);
            for (int i = 0; i < nLights; ++i) {
                PointLight light = pointLights[i];
              
                vec3 lightDir = normalize(vertexPos - light.position);
                vec3 invLightDir = -lightDir;
              
                float diff = max(dot(invLightDir, normal), 0.0);
              
                float distance = length(light.position - vertexPos);
                float factor = light.constant + light.linear * distance + light.quadratic * (distance * distance);
                float attenuation = 1.0 / factor;
              
                vec3 ambient = light.ambient * attenuation;
                vec3 diffuse = light.diffuse * diff * attenuation;
              
                result += (ambient + diffuse);
            }
            return result * vertexColor;
        }
        )";

inline const char* gouraudLitWithInstancedTransformAndVertexColor = R"(
          #version 330
        
          @include "common::constants"
          @include "common::lightsTypes"
        
          uniform DirectionalLight directionalLights[MAX_LIGHTS];
          uniform int nDirectionalLights;
        
          uniform PointLight pointLights[MAX_LIGHTS];
          uniform int nPointLights;
        
          uniform vec3 viewPosition;
        
          uniform mat4 cameraTransform;
        
          layout (location = 0) in vec3 vertexPos;
          layout (location = 1) in vec3 vertexColor;
          layout (location = 2) in vec3 vertexNorm;
          layout (location = 3) in mat4 modelTransform;
        
          out vec3 color;
        
          vec3 CalculateDirectionalLight();
          vec3 CalculatePointLight();
        
          void main()
          {
              vec3 directional = CalculateDirectionalLight();
              vec3 point = CalculatePointLight();
            
              vec3 result = directional + point;
              color = result;
        
              gl_Position = cameraTransform * modelTransform * vec4(vertexPos, 1.0);
          }
        
        
          vec3 CalculateDirectionalLight() {
              vec3 viewDir = normalize(viewPosition - vertexPos);
            
              vec3 normal = normalize(vertexNorm);
        
              vec3 result = vec3(0.0);
              int nLights = min(nDirectionalLights, MAX_LIGHTS);
              for (int i = 0; i < nLights; ++i) {
                  DirectionalLight light = directionalLights[i];
                
                  vec3 lightDir = normalize(light.direction);
                  vec3 invLightDir = -lightDir;
                
                  float diff = max(dot(normal, invLightDir), 0.0);
                
                  vec3 ambient = light.ambient;
                  vec3 diffuse = light.diffuse * diff;
                
                  result += (ambient + diffuse);
              }
              return result * vertexColor; 
          }
        
          vec3 CalculatePointLight() {
              vec3 viewDir = normalize(viewPosition - vertexPos);
              vec3 normal = normalize(vertexNorm);
        
              vec3 result = vec3(0.0);
              int nLights = min(nPointLights, MAX_LIGHTS);
              for (int i = 0; i < nLights; ++i) {
                  PointLight light = pointLights[i];
                
                  vec3 lightDir = normalize(vertexPos - light.position);
                  vec3 invLightDir = -lightDir;
                
                  float diff = max(dot(invLightDir, normal), 0.0);
                
                  float distance = length(light.position - vertexPos);
                  float factor = light.constant + light.linear * distance + light.quadratic * (distance * distance);
                  float attenuation = 1.0 / factor;
                
                  vec3 ambient = light.ambient * attenuation;
                  vec3 diffuse = light.diffuse * diff * attenuation;
                
                  result += (ambient + diffuse);
                
              }
              return result * vertexColor;
          }
          )";

inline const char* textured = R"(
      #version 330

      layout (location = 0) in vec3 pos;
      layout (location = 1) in vec2 uv;

      out vec2 fragUV;

      uniform mat4 cameraTransform;

      void main() {
        gl_Position = cameraTransform * vec4(pos, 1.0);
        fragUV = uv;
      }
  )";

inline const char* texturedWithInstancedTransform = R"(
    #version 330

    layout (location = 0) in vec3 pos;
    layout (location = 1) in vec2 uv;
    layout (location = 2) in mat4 modelTransform;

    out vec2 fragUV;

    uniform mat4 cameraTransform;

    void main() {
      gl_Position = cameraTransform * modelTransform * vec4(pos, 1.0);
      fragUV = uv;
    }
)";
}

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

inline const char* textured = R"(
      #version 330

      in vec2 fragUV;

      out vec4 fragColor;

      uniform sampler2D tex;

      void main() {
        fragColor = texture(tex, fragUV);
      }
    )";

}

}

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------