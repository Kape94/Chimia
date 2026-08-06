#include "Rendering/DataLayout.h"
#include "Rendering/RenderAction.h"
#include "Rendering/Rendering.h"

#include "Rendering/Shader.h"

#include "Utils/Window.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>

namespace Inputs {

constexpr unsigned SCR_WIDTH = 800;
constexpr unsigned SCR_HEIGHT = 600;

namespace ShaderCodes {

const char* vShader = R"(
      #version 330

      layout (location = 0) in vec3 pos;
      layout (location = 1) in vec2 offset;

      void main() {
        gl_Position = vec4(pos, 1.0) + vec4(offset, 0.0, 0.0);
      }
    )";

const char* fShader = R"(
      #version 330

      out vec4 fragColor;

      void main() {
        fragColor = vec4(1.0, 0.5, 0.5, 1.0);
      }
    )";
}

namespace BufferData {

// clang-format off
const std::vector<float> vertex{ // x ,    y,    z
                                 0.0f,  0.0f, 0.0f, 
                                 0.1f, 0.0f,  0.0f,
                                 0.0f,  0.1f, 0.0f, 
                                 0.0f, 0.0f,  0.0f,
                                 -0.1f, 0.0f, 0.0f, 
                                 0.0f, -0.1f, 0.0f
};
// clang-format on

}

namespace InstanceData {

const unsigned dataSize = sizeof(glm::vec2);

const std::vector<glm::vec2> positions{
  { 0.2, 0.2 }, { -0.2, 0.2 }, { -0.2, -0.2 }, { 0.2, -0.2 }, { 0.3, 0.3 },
};

}

}

int
main()
{
  Window win(Inputs::SCR_WIDTH, Inputs::SCR_HEIGHT, "Example #7");

  Chimia::Rendering::Initialize();
  Chimia::Rendering::SetViewport(0, 0, Inputs::SCR_WIDTH, Inputs::SCR_HEIGHT);

  auto shader = Chimia::Rendering::Shader::Create(
    Inputs::ShaderCodes::vShader,
    Inputs::ShaderCodes::fShader,
    { { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
      { "offset", Chimia::Rendering::eDataType::VECTOR_2_FLOAT } });

  auto vertexData = Chimia::Rendering::VertexData::New();
  vertexData->Create(
    Inputs::BufferData::vertex,
    { { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT } });

  auto instancedData = Chimia::Rendering::InstancedData::New();
  instancedData->Create(
    Inputs::InstanceData::positions,
    { { "offset", Chimia::Rendering::eDataType::VECTOR_2_FLOAT } });

  auto target = Chimia::Rendering::Target::Create(shader);

  Chimia::Rendering::RenderAction action;
  action.Create(
    target,
    { { vertexData, "pos", "pos" }, { instancedData, "offset", "offset" } });

  while (!win.ShouldClose()) {
    Chimia::Rendering::Clear();

    action.Render();

    win.Swap();
    win.PollEvents();
  }

  return 0;
}