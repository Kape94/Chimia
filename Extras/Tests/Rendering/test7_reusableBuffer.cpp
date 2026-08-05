#include "Rendering/DataLayout.h"
#include "Rendering/IndexData.h"
#include "Rendering/InstancedData.h"
#include "Rendering/RenderAction.h"
#include "Rendering/Rendering.h"

#include "Rendering/Shader.h"
#include "Rendering/VertexData.h"
#include "TestsUtils.h"
#include "Utils/Window.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>

namespace Scenarios {

void
IndexedBuffer(Window& win)
{
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

  // clang-format off
  //                          x     y     z     r    g    b 
  std::vector<float> vertex{ 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
                             1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
                             0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };
  // clang-format on

  const std::vector<unsigned> indexData{ 0, 1, 2 };

  const Chimia::Rendering::DataLayout dataLayout{
    { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
    { "color", Chimia::Rendering::eDataType::VECTOR_3_FLOAT }
  };

  Chimia::Rendering::Shader shader;
  shader.Create(vShader, fShader, dataLayout);

  auto reusableVertexData = Chimia::Rendering::VertexData::New();
  reusableVertexData->Create(vertex, dataLayout);

  auto reusableIndexData = Chimia::Rendering::IndexData::New();
  reusableIndexData->Create(indexData);

  auto target = Chimia::Rendering::Target::Create(shader);

  Chimia::Rendering::RenderAction renderReusable;
  renderReusable.Create(target,
                        reusableIndexData,
                        { { reusableVertexData, "pos", "pos" },
                          { reusableVertexData, "color", "color" } });

  renderReusable.Render();

  win.Swap();

  TestsUtils::ExpectImage("test7_reusableBuffer_basic.png");
}

void
Instancing(Window& win)
{
  const char* vShaderDisplaced = R"(
      #version 330

      layout (location = 0) in vec3 pos;
      layout (location = 1) in vec2 offset;

      void main() {
        gl_Position = vec4(pos, 1.0) + vec4(offset, 0.0, 0.0);
      }
    )";

  const char* fShaderDisplaced = R"(
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

  // clang-format off
  const std::vector<float> vertex{ // x    y    z
                                  0.0f, 0.0f, 0.0f, 
                                  0.1f, 0.0f, 0.0f, 
                                  0.0f, 0.1f, 0.0f
  };
  // clang-format on
  const unsigned nVertices = 3;

  const std::vector<unsigned> index{ 0, 1, 2 };

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

  Chimia::Rendering::Shader shader1;
  shader1.Create(
    vShaderDisplaced,
    fShaderDisplaced,
    { { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
      { "offset", Chimia::Rendering::eDataType::VECTOR_2_FLOAT } });

  Chimia::Rendering::Shader shader2;
  shader2.Create(
    vShaderTransformed,
    fShaderTransformed,
    { { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
      { "transform", Chimia::Rendering::eDataType::MATRIX_FLOAT_4X4 } });

  auto reusableVertexData = Chimia::Rendering::VertexData::New();
  reusableVertexData->Create(
    vertex, { { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT } });

  auto reusableIndexData = Chimia::Rendering::IndexData::New();
  reusableIndexData->Create(index);

  auto instancedPositions = Chimia::Rendering::InstancedData::New();
  instancedPositions->Create(
    positions, { { "offset", Chimia::Rendering::eDataType::VECTOR_2_FLOAT } });

  auto target1 = Chimia::Rendering::Target::Create(shader1);

  Chimia::Rendering::RenderAction renderReusableWithOffsets;
  renderReusableWithOffsets.Create(
    target1,
    reusableIndexData,
    { { reusableVertexData, "pos", "pos" },
      { instancedPositions, "offset", "offset" } });

  auto instancedTransforms = Chimia::Rendering::InstancedData::New();
  instancedTransforms->Create(
    transforms,
    { { "transform", Chimia::Rendering::eDataType::MATRIX_FLOAT_4X4 } });

  auto target2 = Chimia::Rendering::Target::Create(shader2);

  Chimia::Rendering::RenderAction renderReusableTransformed;
  renderReusableTransformed.Create(
    target2,
    reusableIndexData,
    { { reusableVertexData, "pos", "pos" },
      { instancedTransforms, "transform", "transform" } });

  renderReusableWithOffsets.Render();
  renderReusableTransformed.Render();

  win.Swap();

  TestsUtils::ExpectImage("test7_reusableBuffer_instanced.png");
}

}

int
main(int argc, char** argv)
{
  constexpr unsigned SCR_WIDTH = 800;
  constexpr unsigned SCR_HEIGHT = 600;
  Window win(SCR_WIDTH, SCR_HEIGHT, "Test #7");

  TestsUtils::InitTesting(argv, win);
  Chimia::Rendering::Initialize();

  Scenarios::IndexedBuffer(win);
  Scenarios::Instancing(win);

  return 0;
}