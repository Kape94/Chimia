#include "Rendering/Rendering.h"

#include "Rendering/IndexedRenderAction.h"
#include "Rendering/Shader.h"

#include "Utils/Window.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>

#include <vector>

namespace Inputs {

constexpr unsigned SCR_WIDTH = 800;
constexpr unsigned SCR_HEIGHT = 600;

namespace ShaderCodes {

const char* vShader = R"(
      #version 330

      uniform mat4 transform;

      layout (location = 0) in vec3 pos;

      void main() {
        gl_Position = transform * vec4(pos, 1.0);
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
const std::vector<float> vertex{// x     y    z
                                 0.0f, 0.0f, 0.0f, 
                                 1.0f, 0.0f, 0.0f, 
                                 0.0f, 1.0f, 0.0f
};
// clang-format on
const unsigned nVertices = 3;

const std::vector<unsigned> index{ 0, 1, 2 };

}

}

int
main()
{
  Window win(Inputs::SCR_WIDTH, Inputs::SCR_HEIGHT, "Example #3");

  Chimia::Rendering::Initialize();
  Chimia::Rendering::SetViewport(0, 0, Inputs::SCR_WIDTH, Inputs::SCR_HEIGHT);

  Chimia::Rendering::Shader shader(Inputs::ShaderCodes::vShader,
                                   Inputs::ShaderCodes::fShader);

  auto vertexData = Chimia::Rendering::VertexData::New();
  vertexData->Create(Inputs::BufferData::vertex, Inputs::BufferData::nVertices);

  auto indexData = Chimia::Rendering::IndexData::New();
  indexData->Create(Inputs::BufferData::index);

  Chimia::Rendering::IndexedRenderAction action;
  action.Create(vertexData,
                indexData,
                { Chimia::Rendering::ShaderAttribute::Float(0 /*position*/,
                                                            3 /*nFloats*/) });

  float angle = 0.0f;

  while (!win.ShouldClose()) {
    Chimia::Rendering::Clear();

    shader.Use();

    const glm::mat4x4 identity = glm::identity<glm::mat4x4>();
    const glm::mat4x4 t = glm::rotate(identity, angle, { 0.0, 0.0, 1.0 }) *
                          glm::scale(identity, { 0.25, 0.25, 0.25 });

    shader.SetUniform("transform", t);

    action.Render();

    angle += 0.0005f;
    if (angle > 6.28f)
      angle = 0.0f;

    win.Swap();
    win.PollEvents();
  }

  return 0;
}