#include "Rendering/DataLayout.h"
#include "Rendering/GenericRenderAction.h"
#include "Rendering/Rendering.h"

#include "Rendering/Shader.h"

#include "Rendering/ShaderBinding.h"
#include "Utils/Window.h"

namespace Inputs {

namespace ShaderCodes {

const char* vShader = R"(
      #version 330

      layout (location = 0) in vec3 pos;
      layout (location = 1) in vec3 color;

      out vec3 vertexColor;

      void main() {
        gl_Position = vec4(pos, 1.0);
        vertexColor = color;
      }
    )";

const char* fShader = R"(
      #version 330

      in vec3 vertexColor;

      out vec4 fragColor;

      void main() {
        fragColor = vec4(vertexColor, 1.0);
      }
    )";

}

namespace BufferData {

// clang-format off
const std::vector<float> vertexPositions{ 0.0f, 0.0f, 0.0f,  
                                          1.0f, 0.0f, 0.0f,  
                                          0.0f, 1.0f, 0.0f,  };

const std::vector<float> vertexColors{  0.0f, 1.0f, 0.0f,
                                        1.0f, 0.0f, 0.0f,
                                        0.0f, 0.0f, 1.0f };
// clang-format on

}
}

int
main()
{
  Window win(1280, 1080, "Example #16");

  Chimia::Rendering::Initialize();

  Chimia::Rendering::Shader shader;
  shader.Create(Inputs::ShaderCodes::vShader,
                Inputs::ShaderCodes::fShader,
                { { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
                  { "color", Chimia::Rendering::eDataType::VECTOR_3_FLOAT } });

  auto positionData = Chimia::Rendering::VertexData::New();
  positionData->Create(
    Inputs::BufferData::vertexPositions,
    { { "data", Chimia::Rendering::eDataType::VECTOR_3_FLOAT } });

  auto colorData = Chimia::Rendering::VertexData::New();
  colorData->Create(
    Inputs::BufferData::vertexColors,
    { { "data", Chimia::Rendering::eDataType::VECTOR_3_FLOAT } });

  Chimia::Rendering::GenericRenderAction action;
  action.Create({ Chimia::Rendering::ShaderBinding::Connect(
                    positionData, "data", shader, "pos"),
                  Chimia::Rendering::ShaderBinding::Connect(
                    colorData, "data", shader, "color") });

  while (!win.ShouldClose()) {
    Chimia::Rendering::Clear();
    shader.Use();

    action.Render();

    win.Swap();
    win.PollEvents();
  }

  action.Clear();
  shader.Clear();

  return 0;
}
