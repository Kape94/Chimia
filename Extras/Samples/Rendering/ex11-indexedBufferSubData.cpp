#include "Rendering/IndexData.h"
#include "Rendering/RenderAction.h"
#include "Rendering/Rendering.h"

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

  const Chimia::Rendering::DataLayout dataLayout{
    { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
    { "color", Chimia::Rendering::eDataType::VECTOR_3_FLOAT }
  };

  auto shader = Chimia::Rendering::Shader::Create(
    Inputs::ShaderCodes::vShader, Inputs::ShaderCodes::fShader, dataLayout);

  const auto& states = Inputs::BufferData::states;
  const size_t maximumVertexSize = std::accumulate(
    states.begin(), states.end(), 0, [](size_t current, const auto& incoming) {
      return std::max(current, incoming.vData.size());
    });
  const size_t maximumIndexSize = std::accumulate(
    states.begin(), states.end(), 0, [](size_t current, const auto& incoming) {
      return std::max(current, incoming.iData.size());
    });

  auto vertexData = Chimia::Rendering::VertexData::Create(
    { nullptr, maximumVertexSize * sizeof(float) }, dataLayout);

  auto indexData = Chimia::Rendering::IndexData::Create(
    { nullptr, maximumIndexSize, sizeof(unsigned) });

  auto target = Chimia::Rendering::Target::Create(shader);

  Chimia::Rendering::RenderAction action;
  action.Create(
    target,
    indexData,
    { { vertexData, "pos", "pos" }, { vertexData, "color", "color" } });

  auto changeTime = 1s;
  auto last = std::chrono::high_resolution_clock::now();
  int selected = 0;

  const auto& state = states[selected];

  vertexData->Load(state.vData);
  indexData->LoadIndexData(state.iData);

  while (!win.ShouldClose()) {
    Chimia::Rendering::Clear();

    auto now = std::chrono::high_resolution_clock::now();
    auto diff = now - last;
    if (std::chrono::duration_cast<std::chrono::seconds>(diff) > changeTime) {
      last = now;
      selected = (selected + 1) % states.size();

      const auto& state = states[selected];

      vertexData->Load(state.vData);
      indexData->LoadIndexData(state.iData);
    }

    action.Render();

    win.Swap();
    win.PollEvents();
  }

  return 0;
}
