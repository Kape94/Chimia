#include "Rendering/DataLayout.h"
#include "Rendering/RenderAction.h"
#include "Rendering/Rendering.h"

#include "Rendering/Shader.h"

#include "Utils/Window.h"

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>

namespace Inputs {

constexpr unsigned SCR_WIDTH = 800;
constexpr unsigned SCR_HEIGHT = 600;

namespace ShaderCodes {

const char* vShader = R"(
      #version 330

      layout (location = 0) in vec3 pos;
      layout (location = 1) in mat4 transform;

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

const unsigned dataSize = sizeof(glm::mat4x4);

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
  Window win(Inputs::SCR_WIDTH, Inputs::SCR_HEIGHT, "Example #9");

  Chimia::Rendering::Initialize();
  Chimia::Rendering::SetViewport(0, 0, Inputs::SCR_WIDTH, Inputs::SCR_HEIGHT);

  auto shader = Chimia::Rendering::Shader::Create(
    Inputs::ShaderCodes::vShader,
    Inputs::ShaderCodes::fShader,
    { { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
      { "transform", Chimia::Rendering::eDataType::MATRIX_FLOAT_4X4 } });

  auto vertexData = Chimia::Rendering::VertexData::New();
  vertexData->Create(
    Inputs::BufferData::vertex,
    { { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT } });

  auto instancedData = Chimia::Rendering::InstancedData::New();
  instancedData->Create(
    Inputs::InstanceData::transforms,
    { { "transform", Chimia::Rendering::eDataType::MATRIX_FLOAT_4X4 } });

  auto target = Chimia::Rendering::Target::Create(shader);

  Chimia::Rendering::RenderAction action;
  action.Create(target,
                { { vertexData, "pos", "pos" },
                  { instancedData, "transform", "transform" } });

  while (!win.ShouldClose()) {
    Chimia::Rendering::Clear();

    action.Render();

    win.Swap();
    win.PollEvents();
  }

  return 0;
}