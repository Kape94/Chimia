#include "Rendering/Rendering.h"

#include "Rendering/IndexedVertexBuffer.h"
#include "Rendering/InstancedRenderAction.h"
#include "Rendering/Shader.h"

#include "Rendering/ShaderAttribute.h"
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

const char* vShaderTransformed = R"(
      #version 330

      layout (location = 0) in vec3 pos;
      layout (location = 1) in mat4 transform;

      void main() {
        gl_Position = transform * vec4(pos, 1.0);
      }
    )";

const char* fShaderTransformed = R"(
      #version 330

      out vec4 fragColor;

      void main() {
        fragColor = vec4(0.2, 0.5, 1.0, 1.0);
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
const unsigned nVertices = 3;

const std::vector<unsigned> index{ 0, 1, 2 };
}

namespace InstanceData {

const unsigned dataSize = sizeof(glm::vec2);

const std::vector<glm::vec2> positions{
  { 0.2, 0.2 }, { -0.2, 0.2 }, { -0.2, -0.2 }, { 0.2, -0.2 }, { 0.3, 0.3 },
};

const unsigned transformsDataSize = sizeof(glm::mat4x4);

const glm::mat4x4 m1({ 1.0f, 0.0f, 0.0f, 0.0f },
                     { 0.0f, 1.0f, 0.0f, 0.0f },
                     { 0.0f, 0.0f, 1.0f, 0.0f },
                     { 0.0f, 0.0f, 0.0f, 1.0f });

const glm::mat4x4 m2({ 0.5f, 0.0f, 0.0f, 0.0f },
                     { 0.0f, 1.0f, 0.0f, 0.0f },
                     { 0.0f, 0.0f, 0.5f, 0.0f },
                     { 0.5f, 0.5f, 0.5f, 1.0f });

const std::vector<glm::mat4x4> transforms{ m1, m2 };

}
}

int
main()
{
  Window win(Inputs::SCR_WIDTH, Inputs::SCR_HEIGHT, "Example #13");

  Chimia::Rendering::Initialize();
  Chimia::Rendering::SetViewport(0, 0, Inputs::SCR_WIDTH, Inputs::SCR_HEIGHT);

  Chimia::Rendering::Shader shader1(Inputs::ShaderCodes::vShader,
                                    Inputs::ShaderCodes::fShader);

  Chimia::Rendering::Shader shader2(Inputs::ShaderCodes::vShaderTransformed,
                                    Inputs::ShaderCodes::fShaderTransformed);

  Chimia::Rendering::IndexedVertexBuffer reusableVertexBuffer;
  reusableVertexBuffer.Create(Inputs::BufferData::vertex,
                              Inputs::BufferData::nVertices,
                              Inputs::BufferData::index);

  const Chimia::Rendering::ShaderAttributes vertexAttributes{
    { Chimia::Rendering::ShaderAttribute::Float(0 /*location*/,
                                                3 /*nEntries*/) }
  };
  Chimia::Rendering::InstancedRenderAction renderWithOffsets;
  renderWithOffsets.CreateInstanced(reusableVertexBuffer,
                                    vertexAttributes,
                                    Inputs::InstanceData::positions,
                                    { Chimia::Rendering::ShaderAttribute::Float(
                                      1 /*location*/, 2 /*nEntries*/) });

  Chimia::Rendering::InstancedRenderAction renderTransformed;
  renderTransformed.CreateInstanced(
    reusableVertexBuffer,
    vertexAttributes,
    Inputs::InstanceData::transforms,
    { Chimia::Rendering::ShaderAttribute::Float(1 /*location*/, 4 /*nEntries*/),
      Chimia::Rendering::ShaderAttribute::Float(2 /*location*/, 4 /*nEntries*/),
      Chimia::Rendering::ShaderAttribute::Float(3 /*location*/, 4 /*nEntries*/),
      Chimia::Rendering::ShaderAttribute::Float(4 /*location*/,
                                                4 /*nEntries*/) });

  while (!win.ShouldClose()) {
    Chimia::Rendering::Clear();

    shader1.Use();
    renderWithOffsets.Render();

    shader2.Use();
    renderTransformed.Render();

    win.Swap();
    win.PollEvents();
  }

  return 0;
}