#include "RenderLib/RenderLib.h"

#include "RenderLib/IndexedBuffer.h"
#include "RenderLib/Shader.h"

#include "Utils/Window.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>

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

const std::vector<float> vertex{ // x ,    y,    z
                                 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                                 0.0f, 0.0f, 1.0f, 0.0f
};

const std::vector<unsigned> index{ 0, 1, 2 };

}

}

int
main()
{
  Window win(Inputs::SCR_WIDTH, Inputs::SCR_HEIGHT, "Example #3");

  RenderLib::Initialize();
  RenderLib::SetViewport(0, 0, Inputs::SCR_WIDTH, Inputs::SCR_HEIGHT);

  RenderLib::Shader shader(Inputs::ShaderCodes::vShader,
                           Inputs::ShaderCodes::fShader);

  RenderLib::IndexedBuffer buffer(
    Inputs::BufferData::vertex,
    Inputs::BufferData::index,
    { RenderLib::ShaderAttribute::Float(0 /*position*/, 3 /*nFloats*/) });

  float angle = 0.0f;

  while (!win.ShouldClose()) {
    RenderLib::Clear();

    shader.Use();

    const glm::mat4x4 identity = glm::identity<glm::mat4x4>();
    const glm::mat4x4 t = glm::rotate(identity, angle, { 0.0, 0.0, 1.0 }) *
                          glm::scale(identity, { 0.25, 0.25, 0.25 });

    shader.SetUniform("transform", t);

    buffer.Render();

    angle += 0.0005f;
    if (angle > 6.28f)
      angle = 0.0f;

    win.Swap();
    win.PollEvents();
  }

  return 0;
}