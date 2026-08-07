#include "Rendering/DataLayout.h"
#include "Rendering/IndexData.h"
#include "Rendering/InstancedData.h"
#include "Rendering/RenderAction.h"
#include "Rendering/Rendering.h"

#include "Rendering/Shader.h"

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

  auto shader = Chimia::Rendering::Shader::Create(
    vShader,
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

  auto vertexData = Chimia::Rendering::VertexData::Create(
    vertex, { { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT } });

  auto instancedData = Chimia::Rendering::InstancedData::Create(
    instancesPositions,
    { { "offset", Chimia::Rendering::eDataType::VECTOR_2_FLOAT } });

  auto target = Chimia::Rendering::Target::Create(shader);

  Chimia::Rendering::RenderAction renderWithOffsets;
  renderWithOffsets.Create(
    target,
    { { vertexData, "pos", "pos" }, { instancedData, "offset", "offset" } },
    Chimia::Rendering::ePrimitive::TRIANGLES);

  Chimia::Rendering::Clear();

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

  auto shader = Chimia::Rendering::Shader::Create(
    vShader,
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

  auto vertexData = Chimia::Rendering::VertexData::Create(
    vertex, { { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT } });

  auto indexData = Chimia::Rendering::IndexData::Create(index);

  auto instancedData = Chimia::Rendering::InstancedData::Create(
    instancesPositions,
    { { "offset", Chimia::Rendering::eDataType::VECTOR_2_FLOAT } });

  auto target = Chimia::Rendering::Target::Create(shader);

  Chimia::Rendering::RenderAction renderWithOffsets;
  renderWithOffsets.Create(
    target,
    indexData,
    { { vertexData, "pos", "pos" }, { instancedData, "offset", "offset" } },
    Chimia::Rendering::ePrimitive::TRIANGLES);

  Chimia::Rendering::Clear();

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

  auto shader = Chimia::Rendering::Shader::Create(
    vShader,
    fShader,
    { { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
      { "offset", Chimia::Rendering::eDataType::VECTOR_2_FLOAT } });

  auto vertexData = Chimia::Rendering::VertexData::Create(
    vertex, { { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT } });

  auto instancedData = Chimia::Rendering::InstancedData::Create(
    { nullptr, positions.size() * dataSize },
    { { "offset", Chimia::Rendering::eDataType::VECTOR_2_FLOAT } });

  auto target = Chimia::Rendering::Target::Create(shader);

  Chimia::Rendering::RenderAction renderWithOffsets;
  renderWithOffsets.Create(
    target,
    { { vertexData, "pos", "pos" }, { instancedData, "offset", "offset" } },
    Chimia::Rendering::ePrimitive::TRIANGLES);

  instancedData->Load(positions);

  int selectedGroup = 0;
  for (int selectedGroup = 0; selectedGroup < positionGroups.size();
       ++selectedGroup) {
    Chimia::Rendering::Clear();

    const std::vector<glm::vec2>& positions = positionGroups[selectedGroup];
    instancedData->Load(positions);

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

  auto shader = Chimia::Rendering::Shader::Create(
    vShader,
    fShader,
    { { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
      { "transform", Chimia::Rendering::eDataType::MATRIX_FLOAT_4X4 } });

  auto vertexData = Chimia::Rendering::VertexData::Create(
    vertex, { { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT } });

  auto instancedData = Chimia::Rendering::InstancedData::Create(
    transforms,
    { { "transform", Chimia::Rendering::eDataType::MATRIX_FLOAT_4X4 } });

  auto target = Chimia::Rendering::Target::Create(shader);

  Chimia::Rendering::RenderAction renderTransformed;
  renderTransformed.Create(target,
                           { { vertexData, "pos", "pos" },
                             { instancedData, "transform", "transform" } },
                           Chimia::Rendering::ePrimitive::TRIANGLES);

  Chimia::Rendering::Clear();

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