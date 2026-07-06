#include "Rendering/Rendering.h"

#include "Rendering/IndexedBuffer.h"
#include "Rendering/InstancedBuffer.h"
#include "Rendering/Shader.h"
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

  const unsigned nVertices = 3;
  const std::vector<unsigned> indexData{ 0, 1, 2 };

  Chimia::Rendering::Shader shader;
  shader.Create(vShader, fShader);

  Chimia::Rendering::ReusableIndexedVertexBufferObject reusableVertexBuffer;
  reusableVertexBuffer.Create(vertex, nVertices, indexData);

  Chimia::Rendering::IndexedRenderAction renderReusable;
  renderReusable.Create(
    reusableVertexBuffer,
    { Chimia::Rendering::ShaderAttribute::Float(0 /*position*/, 3 /*nFloats*/),
      Chimia::Rendering::ShaderAttribute::Float(1 /*color*/, 3 /*nFLoats*/) });

  shader.Use();
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

  Chimia::Rendering::Shader shader1(vShaderDisplaced, fShaderDisplaced);
  Chimia::Rendering::Shader shader2(vShaderTransformed, fShaderTransformed);

  Chimia::Rendering::ReusableIndexedVertexBufferObject reusableVertexBuffer;
  reusableVertexBuffer.Create(vertex, nVertices, index);

  const Chimia::Rendering::ShaderAttributes vertexAttributes{
    { Chimia::Rendering::ShaderAttribute::Float(0 /*location*/,
                                                3 /*nEntries*/) }
  };
  Chimia::Rendering::InstancedRenderAction renderReusableWithOffsets;
  renderReusableWithOffsets.CreateInstanced(
    reusableVertexBuffer,
    vertexAttributes,
    positions,
    { Chimia::Rendering::ShaderAttribute::Float(1 /*location*/,
                                                2 /*nEntries*/) });

  Chimia::Rendering::InstancedRenderAction renderReusableTransformed;
  renderReusableTransformed.CreateInstanced(
    reusableVertexBuffer,
    vertexAttributes,
    transforms,
    { Chimia::Rendering::ShaderAttribute::Float(1 /*location*/, 4 /*nEntries*/),
      Chimia::Rendering::ShaderAttribute::Float(2 /*location*/, 4 /*nEntries*/),
      Chimia::Rendering::ShaderAttribute::Float(3 /*location*/, 4 /*nEntries*/),
      Chimia::Rendering::ShaderAttribute::Float(4 /*location*/,
                                                4 /*nEntries*/) });

  shader1.Use();
  renderReusableWithOffsets.Render();

  shader2.Use();
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