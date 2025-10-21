#include "Rendering/Rendering.h"

#include "Rendering/Buffer.h"
#include "Rendering/Shader.h"

#include "Utils/Window.h"

#include <algorithm>
#include <chrono>
#include <numeric>
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
   -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
   0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f};

const std::vector<float> vertex2 = { 
                        // x     y     z     r     g     b
                         0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
                         1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
                         0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                         0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
                         -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
                         0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f};

const std::vector<float> vertex3 = { 
                          // x     y     z     r     g     b
                           0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
                           1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
                           0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                           0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
                           -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
                           0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                           0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 
                           -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 
                           -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};

const std::vector<float> vertex4 = { 
                            // x     y     z     r     g     b
                             0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 
                             -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 
                             0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f};                           

const std::vector<std::vector<float>> vertexStates = { vertex, vertex2, vertex3, vertex4};

// clang-format on
}
}

int
main()
{
  Window win(1280, 1080, "Example #10");
  Chimia::Rendering::Initialize();

  Chimia::Rendering::Shader shader;
  shader.Create(Inputs::ShaderCodes::vShader, Inputs::ShaderCodes::fShader);

  const auto& states = Inputs::BufferData::vertexStates;
  const size_t maximumSize =
    std::accumulate(states.begin(),
                    states.end(),
                    0,
                    [](size_t current, const std::vector<float>& incoming) {
                      return std::max(current, incoming.size());
                    });
  Chimia::Rendering::Buffer buffer;
  buffer.Create(
    nullptr,
    maximumSize * sizeof(float),
    { Chimia::Rendering::ShaderAttribute::Float(0 /*position*/, 3 /*nFloats*/),
      Chimia::Rendering::ShaderAttribute::Float(1 /*color*/, 3 /*nFLoats*/) });

  auto changeTime = 1s;
  auto last = std::chrono::high_resolution_clock::now();
  int selected = 0;

  const std::vector<float>& data = states[selected];
  buffer.Load(data.data(), data.size() * sizeof(float));

  while (!win.ShouldClose()) {
    Chimia::Rendering::Clear();

    auto now = std::chrono::high_resolution_clock::now();
    auto diff = now - last;
    if (std::chrono::duration_cast<std::chrono::seconds>(diff) > changeTime) {
      last = now;
      selected = (selected + 1) % states.size();
      const std::vector<float>& data = states[selected];

      buffer.Load(data.data(), data.size() * sizeof(float));
    }

    shader.Use();
    buffer.Render();

    win.Swap();
    win.PollEvents();
  }

  buffer.Clear();
  shader.Clear();

  return 0;
}
