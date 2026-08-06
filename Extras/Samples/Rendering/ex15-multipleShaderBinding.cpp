#include "Rendering/DataLayout.h"
#include "Rendering/IndexData.h"
#include "Rendering/RenderAction.h"
#include "Rendering/Rendering.h"

#include "Rendering/Shader.h"

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

const std::vector<unsigned> indexData{ 0, 1, 2 };

}
}

int
main()
{
  Window win(1280, 1080, "Example #15");

  Chimia::Rendering::Initialize();

  auto shader = Chimia::Rendering::Shader::Create(
    Inputs::ShaderCodes::vShader,
    Inputs::ShaderCodes::fShader,
    { { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
      { "color", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
      { "target_pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
      { "target_color", Chimia::Rendering::eDataType::VECTOR_3_FLOAT } });

  auto startData = Chimia::Rendering::VertexData::New();
  startData->Create(
    Inputs::BufferData::vertex,
    { { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
      { "color", Chimia::Rendering::eDataType::VECTOR_3_FLOAT } });

  auto targetData = Chimia::Rendering::VertexData::New();
  targetData->Create(
    Inputs::BufferData::vertex_target,
    { { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
      { "color", Chimia::Rendering::eDataType::VECTOR_3_FLOAT } });

  auto reusableIndexData = Chimia::Rendering::IndexData::New();
  reusableIndexData->Create(Inputs::BufferData::indexData);

  auto target = Chimia::Rendering::Target::Create(shader);

  Chimia::Rendering::RenderAction action;
  action.Create(target,
                reusableIndexData,
                { { startData, "pos", "pos" },
                  { startData, "color", "color" },
                  { targetData, "pos", "target_pos" },
                  { targetData, "color", "target_color" } });

  constexpr float SPEED = 0.002f;
  float interpolationRate = 0.0f;
  float increment = SPEED;
  while (!win.ShouldClose()) {
    Chimia::Rendering::Clear();

    shader->SetUniform("interpolation_rate", interpolationRate);
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

  return 0;
}
