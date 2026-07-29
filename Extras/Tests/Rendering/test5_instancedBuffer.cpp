#include "Rendering/DataLayout.h"
#include "Rendering/GenericRenderAction.h"
#include "Rendering/IndexData.h"
#include "Rendering/Rendering.h"

#include "Rendering/InstancedRenderAction.h"
#include "Rendering/Shader.h"

#include "Rendering/ShaderBinding.h"
#include "TestsUtils.h"
#include "Utils/Window.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <string>

namespace Scenarios {

void
Basic(Window& win)
{

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

  Chimia::Rendering::Shader shader;
  shader.Create(vShader,
                fShader,
                { { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
                  { "offset", Chimia::Rendering::eDataType::VECTOR_2_FLOAT } });

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

  const std::vector<glm::vec2> instancesPositions{
    { 0.2, 0.2 }, { -0.2, 0.2 }, { -0.2, -0.2 }, { 0.2, -0.2 }, { 0.3, 0.3 },
  };

  auto vertexData = Chimia::Rendering::VertexData::New();
  vertexData->Create(
    vertex, { { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT } });

  auto instancedData = Chimia::Rendering::InstancedData::New();
  instancedData->Create(
    instancesPositions,
    { { "offset", Chimia::Rendering::eDataType::VECTOR_2_FLOAT } });

  Chimia::Rendering::GenericRenderAction renderWithOffsets;
  renderWithOffsets.Create({ Chimia::Rendering::ShaderBinding::Connect(
                               vertexData, "pos", shader, "pos"),
                             Chimia::Rendering::ShaderBinding::Connect(
                               instancedData, "offset", shader, "offset") });

  Chimia::Rendering::Clear();

  shader.Use();
  renderWithOffsets.Render();

  win.Swap();

  TestsUtils::ExpectImage("test5_instancedBuffer_basic.png");
}

void
BasicIndexed(Window& win)
{
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

  Chimia::Rendering::Shader shader;
  shader.Create(vShader,
                fShader,
                { { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
                  { "offset", Chimia::Rendering::eDataType::VECTOR_2_FLOAT } });

  // clang-format off
  const std::vector<float> vertex{ // x    y    z
                                  0.0f, 0.0f, 0.0f, 
                                  0.1f, 0.0f, 0.0f, 
                                  0.0f, 0.1f, 0.0f
  };
  // clang-format on

  const std::vector<unsigned> index{ 0, 1, 2 };

  const std::vector<glm::vec2> instancesPositions{
    { 0.2, 0.2 }, { -0.2, 0.2 }, { -0.2, -0.2 }, { 0.2, -0.2 }, { 0.3, 0.3 },
  };

  auto vertexData = Chimia::Rendering::VertexData::New();
  vertexData->Create(
    vertex, { { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT } });

  auto indexData = Chimia::Rendering::IndexData::New();
  indexData->Create(index);

  auto instancedData = Chimia::Rendering::InstancedData::New();
  instancedData->Create(
    instancesPositions,
    { { "offset", Chimia::Rendering::eDataType::VECTOR_2_FLOAT } });

  Chimia::Rendering::GenericRenderAction renderWithOffsets;
  renderWithOffsets.Create({ Chimia::Rendering::ShaderBinding::Connect(
                               vertexData, "pos", shader, "pos"),
                             Chimia::Rendering::ShaderBinding::Connect(
                               instancedData, "offset", shader, "offset") },
                           indexData);

  Chimia::Rendering::Clear();

  shader.Use();
  renderWithOffsets.Render();

  win.Swap();

  TestsUtils::ExpectImage("test5_instancedBuffer_basicIndexed.png");
}

void
InstanceSubData(Window& win)
{
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

  const unsigned dataSize = sizeof(glm::vec2);

  const std::vector<glm::vec2> positions{
    { 0.2, 0.2 }, { -0.2, 0.2 }, { -0.2, -0.2 }, { 0.2, -0.2 }, { 0.3, 0.3 },
  };

  const std::vector<glm::vec2> positions2{
    { 0.4, 0.4 }, { -0.4, 0.4 }, { -0.4, -0.4 }, { 0.4, -0.4 }, { 0.3, 0.3 },
  };

  const std::vector<glm::vec2> positions3{
    { 0.3, 0.3 }, { -0.3, 0.3 }, { -0.3, -0.3 }, { 0.3, -0.3 }, { 0.3, 0.3 },
  };

  const std::vector<glm::vec2> positions4{
    { 0.5, 0.5 },
    { -0.5, 0.5 },
  };

  const std::vector<glm::vec2> positions5{ { 0.7, 0.7 } };

  const std::vector<glm::vec2> positions6{ { 0.1, 0.1 },
                                           { 0.1, 0.7 },
                                           { 0.5, 0.5 } };

  const std::vector<std::vector<glm::vec2>> positionGroups{
    positions, positions2, positions3, positions4, positions5, positions6
  };

  Chimia::Rendering::Shader shader;
  shader.Create(vShader,
                fShader,
                { { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
                  { "offset", Chimia::Rendering::eDataType::VECTOR_2_FLOAT } });

  auto vertexData = Chimia::Rendering::VertexData::New();
  vertexData->Create(
    vertex, { { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT } });

  auto instancedData = Chimia::Rendering::InstancedData::New();
  instancedData->Create(
    { nullptr, positions.size() * dataSize },
    { { "offset", Chimia::Rendering::eDataType::VECTOR_2_FLOAT } });

  Chimia::Rendering::GenericRenderAction renderWithOffsets;
  renderWithOffsets.Create({ Chimia::Rendering::ShaderBinding::Connect(
                               vertexData, "pos", shader, "pos"),
                             Chimia::Rendering::ShaderBinding::Connect(
                               instancedData, "offset", shader, "offset") });

  instancedData->Load(positions);

  int selectedGroup = 0;
  for (int selectedGroup = 0; selectedGroup < positionGroups.size();
       ++selectedGroup) {
    Chimia::Rendering::Clear();

    const std::vector<glm::vec2>& positions = positionGroups[selectedGroup];
    instancedData->Load(positions);

    shader.Use();
    renderWithOffsets.Render();

    win.Swap();

    const std::string imageName =
      "test5_instancedBuffer_instancedSubdata_step" +
      std::to_string(selectedGroup) + ".png";

    TestsUtils::ExpectImage(imageName);
  }
}

void
InstancingByTransformMatrix(Window& win)
{
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

  const glm::mat4x4 m1({ 1.0f, 0.0f, 0.0f, 0.0f },
                       { 0.0f, 1.0f, 0.0f, 0.0f },
                       { 0.0f, 0.0f, 1.0f, 0.0f },
                       { 0.0f, 0.0f, 0.0f, 1.0f });

  const glm::mat4x4 m2({ 0.5f, 0.0f, 0.0f, 0.0f },
                       { 0.0f, 1.0f, 0.0f, 0.0f },
                       { 0.0f, 0.0f, 0.5f, 0.0f },
                       { 0.5f, 0.5f, 0.5f, 1.0f });
  const std::vector<glm::mat4x4> transforms{ m1, m2 };

  Chimia::Rendering::Shader shader;
  shader.Create(
    vShader,
    fShader,
    { { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
      { "transform", Chimia::Rendering::eDataType::MATRIX_FLOAT_4X4 } });

  auto vertexData = Chimia::Rendering::VertexData::New();
  vertexData->Create(
    vertex, { { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT } });

  auto instancedData = Chimia::Rendering::InstancedData::New();
  instancedData->Create(
    transforms,
    { { "transform", Chimia::Rendering::eDataType::MATRIX_FLOAT_4X4 } });

  Chimia::Rendering::GenericRenderAction renderTransformed;
  renderTransformed.Create(
    { Chimia::Rendering::ShaderBinding::Connect(
        vertexData, "pos", shader, "pos"),
      Chimia::Rendering::ShaderBinding::Connect(
        instancedData, "transform", shader, "transform") });

  Chimia::Rendering::Clear();

  shader.Use();
  renderTransformed.Render();

  win.Swap();

  TestsUtils::ExpectImage("test5_instancedBuffer_transformMatrix.png");
}

}

int
main(int argc, char** argv)
{
  constexpr unsigned SCR_WIDTH = 800;
  constexpr unsigned SCR_HEIGHT = 600;
  Window win(SCR_WIDTH, SCR_HEIGHT, "Example #5");

  TestsUtils::InitTesting(argv, win);
  Chimia::Rendering::Initialize();

  Scenarios::Basic(win);
  Scenarios::BasicIndexed(win);
  Scenarios::InstanceSubData(win);
  Scenarios::InstancingByTransformMatrix(win);

  return 0;
}