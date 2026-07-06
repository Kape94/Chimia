#include "Rendering/Rendering.h"

#include "Rendering/IndexedBuffer.h"
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

using VertexData = std::vector<float>;
using IndexData = std::vector<unsigned>;

// clang-format off
const VertexData vertex = { 
  // x     y     z     r     g     b
   -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 
   0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 
   0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
   -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f };

const VertexData vertex2 = { 
    // x     y     z     r     g     b
     -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 
     0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f };
  


const IndexData indices = {
  0, 1, 2
};
const IndexData indices2 = {
  0, 1, 3
};

const IndexData indices3 = {
  0, 1, 2,
  2, 3, 0
};

using State = struct {
  const VertexData& vData;
  const IndexData& iData; 
};

const std::vector<State> states = {
  {vertex, indices},
  {vertex, indices2},
  {vertex, indices3},
  {vertex2, indices}
};

// clang-format on
}
}

int
main()
{
  Window win(1280, 1080, "Example #11");
  Chimia::Rendering::Initialize();

  Chimia::Rendering::Shader shader;
  shader.Create(Inputs::ShaderCodes::vShader, Inputs::ShaderCodes::fShader);

  const auto& states = Inputs::BufferData::states;
  const size_t maximumVertexSize = std::accumulate(
    states.begin(), states.end(), 0, [](size_t current, const auto& incoming) {
      return std::max(current, incoming.vData.size());
    });
  const size_t maximumIndexSize = std::accumulate(
    states.begin(), states.end(), 0, [](size_t current, const auto& incoming) {
      return std::max(current, incoming.iData.size());
    });

  Chimia::Rendering::IndexedRenderAction action;
  action.Create(
    { nullptr, maximumVertexSize * sizeof(float) },
    { nullptr, maximumIndexSize, sizeof(unsigned) },
    { Chimia::Rendering::ShaderAttribute::Float(0 /*position*/, 3 /*nFloats*/),
      Chimia::Rendering::ShaderAttribute::Float(1 /*color*/, 3 /*nFLoats*/) });

  auto changeTime = 1s;
  auto last = std::chrono::high_resolution_clock::now();
  int selected = 0;

  const auto& state = states[selected];

  action.LoadVertexData(state.vData);
  action.LoadIndexData(state.iData);

  while (!win.ShouldClose()) {
    Chimia::Rendering::Clear();

    auto now = std::chrono::high_resolution_clock::now();
    auto diff = now - last;
    if (std::chrono::duration_cast<std::chrono::seconds>(diff) > changeTime) {
      last = now;
      selected = (selected + 1) % states.size();

      const auto& state = states[selected];

      action.LoadVertexData(state.vData);
      action.LoadIndexData(state.iData);
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
