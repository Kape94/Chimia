#include "Rendering/DataLayout.h"
#include "Rendering/RenderAction.h"
#include "Rendering/Rendering.h"

#include "Rendering/Shader.h"

#include "Utils/Window.h"

#include "TestsUtils.h"

#include <glm/ext/matrix_float4x4.hpp>
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

const std::vector<unsigned> index{ 0, 1, 2 };

}

}

glm::mat4x4
RotationMatrix(const float angle)
{
  const glm::mat4x4 identity = glm::identity<glm::mat4x4>();
  const glm::mat4x4 t = glm::rotate(identity, angle, { 0.0, 0.0, 1.0 }) *
                        glm::scale(identity, { 0.25, 0.25, 0.25 });
  return t;
}

int
main(int argc, char** argv)
{
  Window win(Inputs::SCR_WIDTH, Inputs::SCR_HEIGHT, "Test #4 - Rendering");

  const auto [scrWidth, scrHeight] = win.GetFramebufferSize();

  TestsUtils::InitTesting(argv, win);

  Chimia::Rendering::Initialize();
  Chimia::Rendering::SetViewport(0, 0, scrWidth, scrHeight);

  const Chimia::Rendering::DataLayout datalayout{
    { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT }
  };

  auto shader = Chimia::Rendering::Shader::Create(
    Inputs::ShaderCodes::vShader, Inputs::ShaderCodes::fShader, datalayout);

  auto vertexData = Chimia::Rendering::VertexData::New();
  vertexData->Create(Inputs::BufferData::vertex, datalayout);

  auto indexData = Chimia::Rendering::IndexData::New();
  indexData->Create(Inputs::BufferData::index);

  auto target = Chimia::Rendering::Target::Create(shader);

  Chimia::Rendering::RenderAction renderTriangle;
  renderTriangle.Create(target, indexData, { { vertexData, "pos", "pos" } });

  constexpr float PI = 3.141592;
  float angle = 0.0f;

  auto draw = [&]() {
    Chimia::Rendering::Clear();

    const glm::mat4x4 t = RotationMatrix(angle);

    shader->SetUniform("transform", t);
    renderTriangle.Render();

    win.Swap();
  };

  draw();
  TestsUtils::ExpectImage("test4_shaderUniform_step1.png");

  angle = 0.25 * PI;

  draw();
  TestsUtils::ExpectImage("test4_shaderUniform_step2.png");

  angle = 0.33 * PI;

  draw();
  TestsUtils::ExpectImage("test4_shaderUniform_step3.png");

  return 0;
}