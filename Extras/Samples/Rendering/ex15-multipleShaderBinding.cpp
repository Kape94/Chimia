#include "Rendering/GenericRenderAction.h"
#include "Rendering/IndexData.h"
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
      layout (location = 2) in vec3 target_pos;
      layout (location = 3) in vec3 target_color;

      out vec3 vertexColor;

      uniform float interpolation_rate;

      void main() {
        vec3 interpolated_pos = pos + (target_pos - pos) * interpolation_rate;
        vec3 interpolated_color = color + (target_color - color) * interpolation_rate;

        gl_Position = vec4(interpolated_pos, 1.0);
        vertexColor = interpolated_color;
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
const std::vector<float> vertex{ 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
                                 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
                                 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };

const std::vector<float> vertex_target{ 0.5f, 0.0f, 0.0f, 0.5f, 1.0f, 0.0f, 
                                        0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.3f, 
                                        0.0f, 0.0f, 0.0f, 0.5f, 0.3f, 1.0f };
// clang-format on

const unsigned nVertices = 3;

const std::vector<unsigned> indexData{ 0, 1, 2 };

}
}

int
main()
{
  Window win(1280, 1080, "Example #15");

  Chimia::Rendering::Initialize();

  Chimia::Rendering::Shader shader;
  shader.Create(Inputs::ShaderCodes::vShader, Inputs::ShaderCodes::fShader);

  Chimia::Rendering::VertexData startData;
  startData.Create(Inputs::BufferData::vertex, Inputs::BufferData::nVertices);

  Chimia::Rendering::VertexData targetData;
  targetData.Create(Inputs::BufferData::vertex_target,
                    Inputs::BufferData::nVertices);

  Chimia::Rendering::IndexData reusableIndexData;
  reusableIndexData.Create(Inputs::BufferData::indexData);

  Chimia::Rendering::GenericRenderAction action;
  action.Create(
    {
      Chimia::Rendering::ShaderBinding::Float(startData, 0 /*pos*/, 3, 0),
      Chimia::Rendering::ShaderBinding::Float(
        startData, 1 /*color*/, 3, 3 * sizeof(float)),
      Chimia::Rendering::ShaderBinding::Float(
        targetData, 2 /*target_pos*/, 3, 0),
      Chimia::Rendering::ShaderBinding::Float(
        targetData, 3 /*target_color*/, 3, 3 * sizeof(float)),
    },
    reusableIndexData);

  constexpr float SPEED = 0.002f;
  float interpolationRate = 0.0f;
  float increment = SPEED;
  while (!win.ShouldClose()) {
    Chimia::Rendering::Clear();
    shader.Use();

    shader.SetUniform("interpolation_rate", interpolationRate);
    action.Render();

    win.Swap();
    win.PollEvents();

    if (interpolationRate < 0.0f) {
      interpolationRate = 0.01f;
      increment = SPEED;
    } else if (interpolationRate > 1.0f) {
      interpolationRate = 0.99f;
      increment = -SPEED;
    }
    interpolationRate += increment;
  }

  action.Clear();
  shader.Clear();

  return 0;
}
