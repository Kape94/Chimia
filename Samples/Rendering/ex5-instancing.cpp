#include "Rendering/Rendering.h"

#include "Rendering/InstancedBuffer.h"
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
const std::vector<float> vertex{ // x    y    z
                                 0.0f, 0.0f, 0.0f, 
                                 0.1f, 0.0f, 0.0f, 
                                 0.0f, 0.1f, 0.0f
};
// clang-format on

const std::vector<unsigned> index{ 0, 1, 2 };
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
  Window win(Inputs::SCR_WIDTH, Inputs::SCR_HEIGHT, "Example #5");

  Chimia::Rendering::Initialize();
  Chimia::Rendering::SetViewport(0, 0, Inputs::SCR_WIDTH, Inputs::SCR_HEIGHT);

  Chimia::Rendering::Shader shader(Inputs::ShaderCodes::vShader,
                                   Inputs::ShaderCodes::fShader);

  Chimia::Rendering::InstancedBuffer buffer;
  buffer.CreateInstanced(Inputs::BufferData::vertex,
                         Inputs::BufferData::index,
                         { Chimia::Rendering::ShaderAttribute::Float(
                           0 /*location*/, 3 /*nEntries*/) },
                         Inputs::InstanceData::positions,
                         { Chimia::Rendering::ShaderAttribute::Float(
                           1 /*location*/, 2 /*nEntries*/) });

  while (!win.ShouldClose()) {
    Chimia::Rendering::Clear();

    shader.Use();
    buffer.Render();

    win.Swap();
    win.PollEvents();
  }

  return 0;
}