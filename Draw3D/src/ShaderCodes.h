#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

namespace ShaderCodes {

namespace Vertex {
inline const char* vertexColored = R"(
            #version 330
      
            layout (location = 0) in vec3 pos;
            layout (location = 1) in vec3 color;
      
            out vec3 vertexColor;
      
            void main() {
              gl_Position = vec4(pos, 1.0);
              vertexColor = color;
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
}

}

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------