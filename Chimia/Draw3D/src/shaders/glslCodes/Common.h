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
        vec3 inputPos, 
        vec3 inputNorm, 
        int nDirectionalLights, 
        DirectionalLight directionalLights[MAX_LIGHTS],
        vec3 mAmbient,
        vec3 mDiffuse,
        vec3 mSpecular,
        float mShininess
    ) 
    {
        vec3 viewDir = normalize(viewPosition - inputPos);
      
        vec3 normal = normalize(inputNorm);
  
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
        vec3 inputPos, 
        vec3 inputNorm, 
        int nDirectionalLights, 
        DirectionalLight directionalLights[MAX_LIGHTS],
        Material material
    ) 
    {
        return CalculateDirectionalLight(
            viewPosition, 
            inputPos, 
            inputNorm, 
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
        vec3 inputPos, 
        vec3 inputNorm, 
        int nPointLights, 
        PointLight pointLights[MAX_LIGHTS],
        vec3 mAmbient,
        vec3 mDiffuse,
        vec3 mSpecular,
        float mShininess
    ) 
    {
        vec3 viewDir = normalize(viewPosition - inputPos);
        vec3 normal = normalize(inputNorm);
  
        vec3 result = vec3(0.0);
        int nLights = min(nPointLights, MAX_LIGHTS);
        for (int i = 0; i < nLights; ++i) {
            PointLight light = pointLights[i];
          
            vec3 lightDir = normalize(inputPos - light.position);
            vec3 invLightDir = -lightDir;
          
            float diff = max(dot(invLightDir, normal), 0.0);
          
            vec3 reflectDir = reflect(lightDir, normal);
            float spec = pow( max(dot(reflectDir, viewDir), 0.0) , mShininess );
          
            float distance = length(light.position - inputPos);
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
      vec3 inputPos, 
      vec3 inputNorm, 
      int nPointLights, 
      PointLight pointLights[MAX_LIGHTS],
      Material material
    ) 
    {
    	return CalculatePointLight(
            viewPosition, 
            inputPos, 
            inputNorm, 
            nPointLights, 
        	pointLights,
            material.ambient,
            material.diffuse,
            material.specular,
            material.shininess
        );
	  }
)";

inline const char* calculateLightsWithoutMaterial = R"(
    @include "common::constants"

    vec3 CalculateDirectionalLight(
      vec3 viewPosition, 
      vec3 inputPos, 
      vec3 inputNorm, 
      int nDirectionalLights, 
      DirectionalLight directionalLights[MAX_LIGHTS],
      vec3 inputColor
    ) 
    {
        vec3 viewDir = normalize(viewPosition - inputPos);
        vec3 normal = normalize(inputNorm);

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
        return result * inputColor; 
    }

    vec3 CalculatePointLight(
        vec3 viewPosition, 
        vec3 inputPos, 
        vec3 inputNorm, 
        int nPointLights, 
        PointLight pointLights[MAX_LIGHTS],
        vec3 inputColor
    ) 
    {
        vec3 viewDir = normalize(viewPosition - inputPos);
        vec3 normal = normalize(inputNorm);

        vec3 result = vec3(0.0);
        int nLights = min(nPointLights, MAX_LIGHTS);
        for (int i = 0; i < nLights; ++i) {
            PointLight light = pointLights[i];
            
            vec3 lightDir = normalize(inputPos - light.position);
            vec3 invLightDir = -lightDir;
            
            float diff = max(dot(invLightDir, normal), 0.0);
            
            float distance = length(light.position - inputPos);
            float factor = light.constant + light.linear * distance + light.quadratic * (distance * distance);
            float attenuation = 1.0 / factor;
            
            vec3 ambient = light.ambient * attenuation;
            vec3 diffuse = light.diffuse * diff * attenuation;
            
            result += (ambient + diffuse);
        }
        return result * inputColor;
    }
)";

}

}

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------