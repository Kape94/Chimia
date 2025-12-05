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
        vec3 pos = vec3(modelTransform * vec4(vertexPos, 1.0));
        vec3 norm = mat3(transpose(inverse(modelTransform))) * vertexNorm;

        vec3 directional = CalculateDirectionalLight(
          viewPosition, 
          pos, 
          norm, 
          nDirectionalLights, 
          directionalLights,
          materialAmbient,
          materialDiffuse,
          materialSpecular,
          materialShininess
        );
        vec3 point = CalculatePointLight(
          viewPosition, 
          pos, 
          norm, 
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
          fragmentPos = vec3(modelTransform * vec4(vertexPos, 1.0));
          fragmentNorm = mat3(transpose(inverse(modelTransform))) * vertexNorm;
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
        @include "common::calculateLightsWithoutMaterial"
      
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
      
        void main()
        {
            vec3 directional = CalculateDirectionalLight(
                                viewPosition, 
                                vertexPos, 
                                vertexNorm, 
                                nDirectionalLights, 
                                directionalLights,
                                vertexColor);

            vec3 point = CalculatePointLight(
                          viewPosition, 
                          vertexPos, 
                          vertexNorm, 
                          nPointLights, 
                          pointLights,
                          vertexColor);
          
            vec3 result = directional + point;
            color = result;
      
            gl_Position = cameraTransform * vec4(vertexPos, 1.0);
        }
        )";

inline const char* gouraudLitWithInstancedTransformAndVertexColor = R"(
          #version 330
        
          @include "common::constants"
          @include "common::lightsTypes"
          @include "common::calculateLightsWithoutMaterial"
        
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
        
          void main()
          {
              vec3 pos = vec3(modelTransform * vec4(vertexPos, 1.0));
              vec3 norm = mat3(transpose(inverse(modelTransform))) * vertexNorm;

              vec3 directional = CalculateDirectionalLight(
                                viewPosition, 
                                pos, 
                                norm, 
                                nDirectionalLights, 
                                directionalLights,
                                vertexColor);

              vec3 point = CalculatePointLight(
                            viewPosition, 
                            pos, 
                            norm, 
                            nPointLights, 
                            pointLights,
                            vertexColor);
            
              vec3 result = directional + point;
              color = result;
        
              gl_Position = cameraTransform * modelTransform * vec4(vertexPos, 1.0);
          }
          )";

inline const char* phongLitWithVertexColor = R"(
            #version 330
                    
            uniform mat4 cameraTransform;
          
            layout (location = 0) in vec3 vertexPos;
            layout (location = 1) in vec3 vertexColor;
            layout (location = 2) in vec3 vertexNorm;
          
            out vec3 fragmentPos;
            out vec3 fragmentColor;
            out vec3 fragmentNorm;
          
            void main()
            {
                fragmentPos = vertexPos;
                fragmentColor = vertexColor;
                fragmentNorm = vertexNorm;
          
                gl_Position = cameraTransform * vec4(vertexPos, 1.0);
            }
            )";

inline const char* phongLitWithInstancedTransformAndVertexColor = R"(
              #version 330
            
              uniform mat4 cameraTransform;
            
              layout (location = 0) in vec3 vertexPos;
              layout (location = 1) in vec3 vertexColor;
              layout (location = 2) in vec3 vertexNorm;
              layout (location = 3) in mat4 modelTransform;
            
              out vec3 fragmentPos;
              out vec3 fragmentColor;
              out vec3 fragmentNorm;
            
              void main()
              {
                  fragmentPos = vec3(modelTransform * vec4(vertexPos, 1.0));
                  fragmentColor = vertexColor;
                  fragmentNorm = mat3(transpose(inverse(modelTransform))) * vertexNorm;
            
                  gl_Position = cameraTransform * modelTransform * vec4(vertexPos, 1.0);
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

inline const char* gouraudLitTextured = R"(
  #version 330

  @include "common::constants"
  @include "common::lightsTypes"
  @include "common::calculateLightsWithoutMaterial"

  uniform DirectionalLight directionalLights[MAX_LIGHTS];
  uniform int nDirectionalLights;

  uniform PointLight pointLights[MAX_LIGHTS];
  uniform int nPointLights;

  uniform vec3 viewPosition;

  uniform mat4 cameraTransform;

  layout (location = 0) in vec3 vertexPos;
  layout (location = 1) in vec3 vertexNorm;
  layout (location = 2) in vec2 vertexTexCoord;

  out vec2 fragmentTexCoord;
  out vec3 fragmentLightColor;

  void main()
  {
      vec3 neutralColor = vec3(1.0, 1.0, 1.0);

      vec3 directional = CalculateDirectionalLight(
                          viewPosition, 
                          vertexPos, 
                          vertexNorm, 
                          nDirectionalLights, 
                          directionalLights,
                          neutralColor);

      vec3 point = CalculatePointLight(
                    viewPosition, 
                    vertexPos, 
                    vertexNorm, 
                    nPointLights, 
                    pointLights,
                    neutralColor);
    
      vec3 result = directional + point;
      
      fragmentLightColor = result;
      fragmentTexCoord = vertexTexCoord;

      gl_Position = cameraTransform * vec4(vertexPos, 1.0);
  }
  )";

inline const char* gouraudLitTexturedWithInstancedTransform = R"(
    #version 330

    @include "common::constants"
    @include "common::lightsTypes"
    @include "common::calculateLightsWithoutMaterial"

    uniform DirectionalLight directionalLights[MAX_LIGHTS];
    uniform int nDirectionalLights;

    uniform PointLight pointLights[MAX_LIGHTS];
    uniform int nPointLights;

    uniform vec3 viewPosition;
    uniform mat4 cameraTransform;

    layout (location = 0) in vec3 vertexPos;
    layout (location = 1) in vec3 vertexNorm;
    layout (location = 2) in vec2 vertexTexCoord;
    layout (location = 3) in mat4 modelTransform;

    out vec2 fragmentTexCoord;
    out vec3 fragmentLightColor;

    void main() {
      vec3 pos = vec3(modelTransform * vec4(vertexPos, 1.0));
      vec3 norm = mat3(transpose(inverse(modelTransform))) * vertexNorm;
      vec3 neutralColor = vec3(1.0, 1.0, 1.0);

      vec3 directional = CalculateDirectionalLight(
                        viewPosition, 
                        pos, 
                        norm, 
                        nDirectionalLights, 
                        directionalLights,
                        neutralColor);

      vec3 point = CalculatePointLight(
                    viewPosition, 
                    pos, 
                    norm, 
                    nPointLights, 
                    pointLights,
                    neutralColor);

      vec3 result = directional + point;
      
      fragmentLightColor = result;
      fragmentTexCoord = vertexTexCoord;

      gl_Position = cameraTransform * modelTransform * vec4(vertexPos, 1.0);
    }
)";

inline const char* phongLitTextured = R"(
  #version 330

  uniform mat4 cameraTransform;

  layout (location = 0) in vec3 vertexPos;
  layout (location = 1) in vec3 vertexNorm;
  layout (location = 2) in vec2 vertexTexCoord;

  out vec3 fragmentPos;
  out vec2 fragmentTexCoord;
  out vec3 fragmentNorm;

  void main()
  {
      fragmentPos = vertexPos;
      fragmentNorm = vertexNorm;
      fragmentTexCoord = vertexTexCoord;

      gl_Position = cameraTransform * vec4(vertexPos, 1.0);
  }
  )";

inline const char* phongLitTexturedWithInstancedTransform = R"(
    #version 330

    uniform mat4 cameraTransform;

    layout (location = 0) in vec3 vertexPos;
    layout (location = 1) in vec3 vertexNorm;
    layout (location = 2) in vec2 vertexTexCoord;
    layout (location = 3) in mat4 modelTransform;

    out vec3 fragmentPos;
    out vec2 fragmentTexCoord;
    out vec3 fragmentNorm;

    void main() {
      fragmentPos = vec3(modelTransform * vec4(vertexPos, 1.0));
      fragmentNorm = mat3(transpose(inverse(modelTransform))) * vertexNorm;
      fragmentTexCoord = vertexTexCoord;

      gl_Position = cameraTransform * modelTransform * vec4(vertexPos, 1.0);
    }
)";

}

}

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------