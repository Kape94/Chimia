#include "Rendering/Rendering.h"

#include "Rendering/InstancedBuffer.h"
#include "Rendering/Shader.h"

#include "TestsUtils.h"
#include "Utils/SamplesUtils.h"
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

  Chimia::Rendering::Shader shader(vShader, fShader);

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

  Chimia::Rendering::InstancedBuffer buffer;
  buffer.CreateInstanced(vertex,
                         { Chimia::Rendering::ShaderAttribute::Float(
                           0 /*location*/, 3 /*nEntries*/) },
                         instancesPositions,
                         { Chimia::Rendering::ShaderAttribute::Float(
                           1 /*location*/, 2 /*nEntries*/) });

  Chimia::Rendering::Clear();

  shader.Use();
  buffer.Render();

  win.Swap();

  TestsUtils::ExpectImage("test5_instancedBuffer_basic.bmp");
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

  Chimia::Rendering::Shader shader(vShader, fShader);

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

  Chimia::Rendering::InstancedBuffer buffer;
  buffer.CreateInstanced(vertex,
                         index,
                         { Chimia::Rendering::ShaderAttribute::Float(
                           0 /*location*/, 3 /*nEntries*/) },
                         instancesPositions,
                         { Chimia::Rendering::ShaderAttribute::Float(
                           1 /*location*/, 2 /*nEntries*/) });

  Chimia::Rendering::Clear();

  shader.Use();
  buffer.Render();

  win.Swap();

  TestsUtils::ExpectImage("test5_instancedBuffer_basicIndexed.bmp");
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

  Chimia::Rendering::Shader shader(vShader, fShader);

  Chimia::Rendering::InstancedBuffer buffer;
  buffer.CreateInstanced(vertex,
                         { Chimia::Rendering::ShaderAttribute::Float(
                           0 /*location*/, 3 /*nEntries*/) },
                         { nullptr, positions.size(), dataSize },
                         { Chimia::Rendering::ShaderAttribute::Float(
                           1 /*location*/, 2 /*nEntries*/) });

  buffer.LoadInstancedData(positions);

  int selectedGroup = 0;
  for (int selectedGroup = 0; selectedGroup < positionGroups.size();
       ++selectedGroup) {
    Chimia::Rendering::Clear();

    const std::vector<glm::vec2>& positions = positionGroups[selectedGroup];
    buffer.LoadInstancedData(positions);

    shader.Use();
    buffer.Render();

    win.Swap();

    const std::string imageName =
      "test5_instancedBuffer_instancedSubdata_step" +
      std::to_string(selectedGroup) + ".bmp";

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

  Chimia::Rendering::Shader shader(vShader, fShader);

  Chimia::Rendering::InstancedBuffer buffer;
  buffer.CreateInstanced(
    vertex,
    { Chimia::Rendering::ShaderAttribute::Float(0 /*location*/,
                                                3 /*nEntries*/) },
    transforms,
    { Chimia::Rendering::ShaderAttribute::Float(1 /*location*/, 4 /*nEntries*/),
      Chimia::Rendering::ShaderAttribute::Float(2 /*location*/, 4 /*nEntries*/),
      Chimia::Rendering::ShaderAttribute::Float(3 /*location*/, 4 /*nEntries*/),
      Chimia::Rendering::ShaderAttribute::Float(4 /*location*/,
                                                4 /*nEntries*/) });

  Chimia::Rendering::Clear();

  shader.Use();
  buffer.Render();

  win.Swap();

  TestsUtils::ExpectImage("test5_instancedBuffer_transformMatrix.bmp");
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