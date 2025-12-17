#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

namespace ShaderCodes {

namespace Vertex {

inline const char* colored = R"(
    #version 330

    @include "vertexInputLayout::P3C3"

    out vec3 fragmentColor;

    uniform mat4 cameraTransform;

    void main() {
      gl_Position = cameraTransform * vec4(vertexPos, 1.0);
      fragmentColor = vertexColor;
    }
  )";

inline const char* coloredWithInstancedTransform = R"(
    #version 330

    @include "vertexInputLayout::P3C3Instanced"

    out vec3 fragmentColor;

    uniform mat4 cameraTransform;

    void main() {
      gl_Position = cameraTransform * instanceTransform * vec4(vertexPos, 1.0);
      fragmentColor = vertexColor;
    }
  )";

inline const char* gouraudLit = R"(
  #version 330

  @include "common::constants"
  @include "common::lightsTypes"
  @include "common::materialType"
  @include "common::calculateLights"

  uniform Material material;

  @include "common::lightsUniforms"

  uniform mat4 cameraTransform;

  @include "vertexInputLayout::P3N3"

  out vec3 fragmentColor;

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
	    fragmentColor = result;

      gl_Position = cameraTransform * vec4(vertexPos, 1.0);
  }
  )";

inline const char* gouraudLitWithInstancedTransformAndMaterial = R"(
    #version 330
  
    @include "common::constants"
    @include "common::lightsTypes"
    @include "common::materialType"
    @include "common::calculateLights"
  
    uniform Material material;

    @include "common::lightsUniforms"
  
    uniform mat4 cameraTransform;
  
    @include "vertexInputLayout::P3N3Instanced"
  
    out vec3 fragmentColor;
  
    void main()
    {
        vec3 pos = vec3(instanceTransform * vec4(vertexPos, 1.0));
        vec3 norm = mat3(transpose(inverse(instanceTransform))) * vertexNorm;

        vec3 directional = CalculateDirectionalLight(
          viewPosition, 
          pos, 
          norm, 
          nDirectionalLights, 
          directionalLights,
          material
        );
        vec3 point = CalculatePointLight(
          viewPosition, 
          pos, 
          norm, 
          nPointLights, 
          pointLights,
          material
        );
      
        vec3 result = directional + point;
        fragmentColor = result;
  
        gl_Position = cameraTransform * instanceTransform * vec4(vertexPos, 1.0);
    }
    )";

inline const char* phongLit = R"(
      #version 330
    
      @include "vertexInputLayout::P3N3"

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
    
      @include "vertexInputLayout::P3N3Instanced"
    
      out vec3 fragmentPos;
      out vec3 fragmentNorm;

      uniform mat4 cameraTransform;

      void main()
      {
          fragmentPos = vec3(instanceTransform * vec4(vertexPos, 1.0));
          fragmentNorm = mat3(transpose(inverse(instanceTransform))) * vertexNorm;

          gl_Position = cameraTransform * instanceTransform * vec4(vertexPos, 1.0);
      }
      )";

inline const char* gouraudLitWithVertexColor = R"(
        #version 330
      
        @include "common::constants"
        @include "common::lightsTypes"
        @include "common::calculateLightsWithoutMaterial"
      
        @include "common::lightsUniforms"
      
        uniform mat4 cameraTransform;
      
        @include "vertexInputLayout::P3C3N3"
      
        out vec3 fragmentColor;
      
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
            fragmentColor = result;
      
            gl_Position = cameraTransform * vec4(vertexPos, 1.0);
        }
        )";

inline const char* gouraudLitWithInstancedTransformAndVertexColor = R"(
          #version 330
        
          @include "common::constants"
          @include "common::lightsTypes"
          @include "common::calculateLightsWithoutMaterial"
        
          @include "common::lightsUniforms"
        
          uniform mat4 cameraTransform;
        
          @include "vertexInputLayout::P3C3N3Instanced"
        
          out vec3 fragmentColor;
        
          void main()
          {
              vec3 pos = vec3(instanceTransform * vec4(vertexPos, 1.0));
              vec3 norm = mat3(transpose(inverse(instanceTransform))) * vertexNorm;

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
              fragmentColor = result;
        
              gl_Position = cameraTransform * instanceTransform * vec4(vertexPos, 1.0);
          }
          )";

inline const char* phongLitWithVertexColor = R"(
            #version 330
                    
            uniform mat4 cameraTransform;
          
            @include "vertexInputLayout::P3C3N3"
          
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
            
              @include "vertexInputLayout::P3C3N3Instanced"
            
              out vec3 fragmentPos;
              out vec3 fragmentColor;
              out vec3 fragmentNorm;
            
              void main()
              {
                  fragmentPos = vec3(instanceTransform * vec4(vertexPos, 1.0));
                  fragmentColor = vertexColor;
                  fragmentNorm = mat3(transpose(inverse(instanceTransform))) * vertexNorm;
            
                  gl_Position = cameraTransform * instanceTransform * vec4(vertexPos, 1.0);
              }
              )";

inline const char* textured = R"(
      #version 330

      @include "vertexInputLayout::P3T2"

      out vec2 fragmentTexCoord;

      uniform mat4 cameraTransform;

      void main() {
        gl_Position = cameraTransform * vec4(vertexPos, 1.0);
        fragmentTexCoord = vertexTexCoord;
      }
  )";

inline const char* texturedWithInstancedTransform = R"(
    #version 330

    @include "vertexInputLayout::P3T2Instanced"

    out vec2 fragmentTexCoord;

    uniform mat4 cameraTransform;

    void main() {
      gl_Position = cameraTransform * instanceTransform * vec4(vertexPos, 1.0);
      fragmentTexCoord = vertexTexCoord;
    }
)";

inline const char* coloredTextured = R"(
  #version 330

  @include "vertexInputLayout::P3C3T2"

  out vec3 fragmentColor;
  out vec2 fragmentTexCoord;

  uniform mat4 cameraTransform;

  void main() {
    gl_Position = cameraTransform * vec4(vertexPos, 1.0);

    fragmentColor = vertexColor;
    fragmentTexCoord = vertexTexCoord;
  }
)";

inline const char* coloredTexturedWithInstancedTransform = R"(
#version 330

@include "vertexInputLayout::P3C3T2Instanced"

out vec3 fragmentColor;
out vec2 fragmentTexCoord;

uniform mat4 cameraTransform;

void main() {
  gl_Position = cameraTransform * instanceTransform * vec4(vertexPos, 1.0);

  fragmentColor = vertexColor;
  fragmentTexCoord = vertexTexCoord;
}
)";

inline const char* gouraudLitTextured = R"(
  #version 330

  @include "common::constants"
  @include "common::lightsTypes"
  @include "common::calculateLightsWithoutMaterial"

  @include "common::lightsUniforms"

  uniform mat4 cameraTransform;

  @include "vertexInputLayout::P3N3T2"

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

    @include "common::lightsUniforms"
    uniform mat4 cameraTransform;

    @include "vertexInputLayout::P3N3T2Instanced"

    out vec2 fragmentTexCoord;
    out vec3 fragmentLightColor;

    void main() {
      vec3 pos = vec3(instanceTransform * vec4(vertexPos, 1.0));
      vec3 norm = mat3(transpose(inverse(instanceTransform))) * vertexNorm;
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

      gl_Position = cameraTransform * instanceTransform * vec4(vertexPos, 1.0);
    }
)";

inline const char* phongLitTextured = R"(
  #version 330

  uniform mat4 cameraTransform;

  @include "vertexInputLayout::P3N3T2"

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

    @include "vertexInputLayout::P3N3T2Instanced"

    out vec3 fragmentPos;
    out vec3 fragmentNorm;
    out vec2 fragmentTexCoord;

    void main() {
      fragmentPos = vec3(instanceTransform * vec4(vertexPos, 1.0));
      fragmentNorm = mat3(transpose(inverse(instanceTransform))) * vertexNorm;
      fragmentTexCoord = vertexTexCoord;

      gl_Position = cameraTransform * instanceTransform * vec4(vertexPos, 1.0);
    }
)";

inline const char* gouraudLitColoredTextured = R"(
  #version 330

  @include "common::constants"
  @include "common::lightsTypes"
  @include "common::calculateLightsWithoutMaterial"

  @include "common::lightsUniforms"

  uniform mat4 cameraTransform;

  @include "vertexInputLayout::P3C3N3T2"

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
      
      fragmentLightColor = result * vertexColor;
      fragmentTexCoord = vertexTexCoord;

      gl_Position = cameraTransform * vec4(vertexPos, 1.0);
  }
  )";

inline const char* gouraudLitColoredTexturedWithInstancedTransform = R"(
    #version 330

    @include "common::constants"
    @include "common::lightsTypes"
    @include "common::calculateLightsWithoutMaterial"

    @include "common::lightsUniforms"
    uniform mat4 cameraTransform;

    @include "vertexInputLayout::P3C3N3T2Instanced"

    out vec2 fragmentTexCoord;
    out vec3 fragmentLightColor;

    void main() {
      vec3 pos = vec3(instanceTransform * vec4(vertexPos, 1.0));
      vec3 norm = mat3(transpose(inverse(instanceTransform))) * vertexNorm;
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
      
      fragmentLightColor = result * vertexColor;
      fragmentTexCoord = vertexTexCoord;

      gl_Position = cameraTransform * instanceTransform * vec4(vertexPos, 1.0);
    }
)";

inline const char* phongLitColoredTextured = R"(
  #version 330

  uniform mat4 cameraTransform;

  @include "vertexInputLayout::P3C3N3T2"

  out vec3 fragmentPos;
  out vec3 fragmentColor;
  out vec3 fragmentNorm;
  out vec2 fragmentTexCoord;

  void main()
  {
      fragmentPos = vertexPos;
      fragmentColor = vertexColor;
      fragmentNorm = vertexNorm;
      fragmentTexCoord = vertexTexCoord;

      gl_Position = cameraTransform * vec4(vertexPos, 1.0);
  }
  )";

inline const char* phongLitColoredTexturedWithInstancedTransform = R"(
    #version 330

    uniform mat4 cameraTransform;

    @include "vertexInputLayout::P3C3N3T2Instanced"

    out vec3 fragmentPos;
    out vec3 fragmentColor;
    out vec2 fragmentTexCoord;
    out vec3 fragmentNorm;

    void main() {
      fragmentPos = vec3(instanceTransform * vec4(vertexPos, 1.0));
      fragmentColor = vertexColor;
      fragmentNorm = mat3(transpose(inverse(instanceTransform))) * vertexNorm;
      fragmentTexCoord = vertexTexCoord;

      gl_Position = cameraTransform * instanceTransform * vec4(vertexPos, 1.0);
    }
)";

}

}

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------