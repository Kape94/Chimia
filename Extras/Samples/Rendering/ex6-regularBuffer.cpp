#include "Rendering/DataLayout.h"
#include "Rendering/RenderAction.h"
#include "Rendering/Rendering.h"

#include "Rendering/Shader.h"

#include "Rendering/ShaderBinding.h"
#include "Utils/Window.h"

#include <chrono>
#include <vector>

using namespace std::literals;

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
const std::vector<float> vertex = { 
                        // x     y     z     r     g     b
                         0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
                         1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
                         0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                         0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
                         -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
                         0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f};

const std::vector<float> vertex2 = { 
                          // x     y     z     r     g     b
                           0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
                           -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
                           0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
                           0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
                           1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
                           0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
// clang-format on
}
}

int
main()
{
  Window win(1280, 1080, "Example #6");
  Chimia::Rendering::Initialize();

  const Chimia::Rendering::DataLayout dataLayout(
    { { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
      { "color", Chimia::Rendering::eDataType::VECTOR_3_FLOAT } });

  Chimia::Rendering::Shader shader;
  shader.Create(
    Inputs::ShaderCodes::vShader, Inputs::ShaderCodes::fShader, dataLayout);

  auto vertexData = Chimia::Rendering::VertexData::New();
  vertexData->Create(Inputs::BufferData::vertex, dataLayout);

  Chimia::Rendering::RenderAction action;
  action.Create({ Chimia::Rendering::ShaderBinding::Connect(
                    vertexData, "pos", shader, "pos"),
                  Chimia::Rendering::ShaderBinding::Connect(
                    vertexData, "color", shader, "color") });

  auto changeTime = 1s;
  auto last = std::chrono::high_resolution_clock::now();
  int selected = 0;

  while (!win.ShouldClose()) {
    Chimia::Rendering::Clear();

    auto now = std::chrono::high_resolution_clock::now();
    auto diff = now - last;
    if (std::chrono::duration_cast<std::chrono::seconds>(diff) > changeTime) {
      last = now;
      selected = (selected + 1) % 2;
      const std::vector<float>& data = selected == 0
                                         ? Inputs::BufferData::vertex
                                         : Inputs::BufferData::vertex2;
      vertexData->Load(data);
    }

    shader.Use();
    action.Render();

    win.Swap();
    win.PollEvents();
  }

  action.Clear();
  shader.Clear();

  return 0;
}
