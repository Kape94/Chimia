#include "Rendering/DataLayout.h"
#include "Rendering/GenericRenderAction.h"
#include "Rendering/IndexData.h"
#include "Rendering/Rendering.h"

#include "Rendering/Shader.h"

#include "Rendering/ShaderBinding.h"
#include "Rendering/VertexData.h"
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

  Chimia::Rendering::Shader shader1;
  shader1.Create(
    Inputs::ShaderCodes::vShader,
    Inputs::ShaderCodes::fShader,
    { { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
      { "offset", Chimia::Rendering::eDataType::VECTOR_2_FLOAT } });

  Chimia::Rendering::Shader shader2;
  shader2.Create(
    Inputs::ShaderCodes::vShaderTransformed,
    Inputs::ShaderCodes::fShaderTransformed,
    { { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
      { "transform", Chimia::Rendering::eDataType::MATRIX_FLOAT_4X4 } });

  auto reusableVertexData = Chimia::Rendering::VertexData::New();
  reusableVertexData->Create(
    Inputs::BufferData::vertex,
    { { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT } });

  auto reusableIndexData = Chimia::Rendering::IndexData::New();
  reusableIndexData->Create(Inputs::BufferData::index);

  auto positionsData = Chimia::Rendering::InstancedData::New();
  positionsData->Create(
    Inputs::InstanceData::positions,
    { { "offset", Chimia::Rendering::eDataType::VECTOR_2_FLOAT } });

  Chimia::Rendering::GenericRenderAction renderWithOffsets;
  renderWithOffsets.Create({ Chimia::Rendering::ShaderBinding::Connect(
                               reusableVertexData, "pos", shader1, "pos"),
                             Chimia::Rendering::ShaderBinding::Connect(
                               positionsData, "offset", shader1, "offset") },
                           reusableIndexData);

  auto transformData = Chimia::Rendering::InstancedData::New();
  transformData->Create(
    Inputs::InstanceData::transforms,
    { { "transform", Chimia::Rendering::eDataType::MATRIX_FLOAT_4X4 } });

  Chimia::Rendering::GenericRenderAction renderTransformed;
  renderTransformed.Create(
    { Chimia::Rendering::ShaderBinding::Connect(
        reusableVertexData, "pos", shader2, "pos"),
      Chimia::Rendering::ShaderBinding::Connect(
        transformData, "transform", shader2, "transform") },
    reusableIndexData);

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