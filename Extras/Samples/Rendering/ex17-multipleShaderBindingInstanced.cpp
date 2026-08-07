#include "Rendering/DataLayout.h"
#include "Rendering/InstancedData.h"
#include "Rendering/RenderAction.h"
#include "Rendering/Rendering.h"

#include "Rendering/Shader.h"

#include "Utils/Window.h"
#include <glm/ext/vector_float3.hpp>

namespace Inputs {

namespace ShaderCodes {

const char* vShader = R"(
      #version 330

      layout (location = 0) in vec3 pos;
      layout (location = 1) in vec3 color;
      layout (location = 2) in vec3 offset;

      out vec3 vertexColor;

      void main() {
        gl_Position = vec4(pos + offset, 1.0);
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

const std::vector<glm::vec3> offsets{ { 0.0f, 0.0f, 0.0f },
                                      { -1.0f, 0.0f, 0.0f } };

}
}

int
main()
{
  Window win(1280, 1080, "Example #16");

  Chimia::Rendering::Initialize();

  auto shader = Chimia::Rendering::Shader::Create(
    Inputs::ShaderCodes::vShader,
    Inputs::ShaderCodes::fShader,
    { { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
      { "color", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
      { "offset", Chimia::Rendering::eDataType::VECTOR_3_FLOAT } });

  auto positionData = Chimia::Rendering::VertexData::Create(
    Inputs::BufferData::vertexPositions,
    { { "data", Chimia::Rendering::eDataType::VECTOR_3_FLOAT } });

  auto colorData = Chimia::Rendering::VertexData::Create(
    Inputs::BufferData::vertexColors,
    { { "data", Chimia::Rendering::eDataType::VECTOR_3_FLOAT } });

  auto offsetData = Chimia::Rendering::InstancedData::Create(
    Inputs::BufferData::offsets,
    { { "data", Chimia::Rendering::eDataType::VECTOR_3_FLOAT } });

  auto target = Chimia::Rendering::Target::Create(shader);

  Chimia::Rendering::RenderAction action;
  action.Create(target,
                {
                  { positionData, "data", "pos" },
                  { colorData, "data", "color" },
                  { offsetData, "data", "offset" },
                });

  while (!win.ShouldClose()) {
    Chimia::Rendering::Clear();

    action.Render();

    win.Swap();
    win.PollEvents();
  }

  return 0;
}
