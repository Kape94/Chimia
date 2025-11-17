#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

namespace ShaderCodes {

namespace Vertex {

inline const char* colored = R"(
    #version 330

    layout (location = 0) in vec3 pos;
    layout (location = 1) in vec3 color;

    out vec3 vertexColor;

    uniform mat4 transform;

    void main() {
      gl_Position = transform * vec4(pos, 1.0);
      vertexColor = color;
    }
  )";

inline const char* coloredWithInstancedTransform = R"(
    #version 330

    layout (location = 0) in vec3 pos;
    layout (location = 1) in vec3 color;
    layout (location = 2) in mat4 modelTransform;

    out vec3 vertexColor;

    uniform mat4 transform;

    void main() {
      gl_Position = transform * modelTransform * vec4(pos, 1.0);
      vertexColor = color;
    }
  )";

inline const char* gouraudLit = R"(
  #version 330

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

  struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
  };

  #define MAX_LIGHTS 4

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
        
          vec3 reflectDir = reflect(lightDir, normal);
          float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
        
          vec3 ambient = light.ambient * material.ambient;
          vec3 diffuse = light.diffuse * (diff * material.diffuse);
          vec3 specular = light.specular * (spec * material.specular);
        
          result += (ambient + diffuse + specular);
      }
      return result; 
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
        
          vec3 reflectDir = reflect(lightDir, normal);
          float spec = pow( max(dot(reflectDir, viewDir), 0.0) , material.shininess );
        
          float distance = length(light.position - vertexPos);
          float factor = light.constant + light.linear * distance + light.quadratic * (distance * distance);
          float attenuation = 1.0 / factor;
        
          vec3 ambient = (light.ambient * material.ambient) * attenuation;
          vec3 diffuse = ( light.diffuse * (diff * material.diffuse) ) * attenuation;
          vec3 specular = ( light.specular * (spec * material.specular) ) * attenuation;
        
          result += (ambient + diffuse + specular);
        
      }
      return result;
  }
  )";

inline const char* gouraudLitWithInstancedTransformAndMaterial = R"(
    #version 330
  
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
  
    struct Material {
      vec3 ambient;
      vec3 diffuse;
      vec3 specular;
      float shininess;
    };
  
    #define MAX_LIGHTS 4
  
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
          
            vec3 reflectDir = reflect(lightDir, normal);
            float spec = pow(max(dot(reflectDir, viewDir), 0.0), materialShininess);
          
            vec3 ambient = light.ambient * materialAmbient;
            vec3 diffuse = light.diffuse * (diff * materialDiffuse);
            vec3 specular = light.specular * (spec * materialSpecular);
          
            result += (ambient + diffuse + specular);
        }
        return result; 
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
          
            vec3 reflectDir = reflect(lightDir, normal);
            float spec = pow( max(dot(reflectDir, viewDir), 0.0) , materialShininess );
          
            float distance = length(light.position - vertexPos);
            float factor = light.constant + light.linear * distance + light.quadratic * (distance * distance);
            float attenuation = 1.0 / factor;
          
            vec3 ambient = (light.ambient * materialAmbient) * attenuation;
            vec3 diffuse = ( light.diffuse * (diff * materialDiffuse) ) * attenuation;
            vec3 specular = ( light.specular * (spec * materialSpecular) ) * attenuation;
          
            result += (ambient + diffuse + specular);
          
        }
        return result;
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

}

}

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------