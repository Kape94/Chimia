#include "Rendering/Rendering.h"

#include "Rendering/IndexedBuffer.h"
#include "Rendering/Shader.h"

#include "Utils/Window.h"

namespace Inputs {

namespace ShaderCodes {

const char* vShader = R"(
      #version 330

      layout (location = 0) in vec3 pos;
      layout (location = 1) in vec4 color;

      out vec4 vertexColor;

      void main() {
        gl_Position = vec4(pos, 1.0);
        vertexColor = color;
      }
    )";

const char* fShader = R"(
      #version 330

      in vec4 vertexColor;

      out vec4 fragColor;

      void main() {
        fragColor = vertexColor;
      }
    )";

}

namespace BufferData {

// clang-format off
const float vertex[] = { 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                         1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f };
// clang-format on

const unsigned vertexDataSize = 21 * sizeof(float);

const unsigned indexData[] = { 0, 1, 2 };

const unsigned indexDataNItems = 3;

const float alpha = 0.3f;
// clang-format off
const float vertex2[] = { -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, alpha,
                         1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, alpha,
                        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, alpha };
// clang-format on

const unsigned vertex2DataSize = 21 * sizeof(float);

const unsigned index2Data[] = { 0, 1, 2 };

const unsigned index2DataNItems = 3;
}
}

int
main()
{
  Window win(1280, 1080, "Example #1");

  Chimia::Rendering::Initialize();
  Chimia::Rendering::EnableColorBlend(true);

  Chimia::Rendering::Shader shader;
  shader.Create(Inputs::ShaderCodes::vShader, Inputs::ShaderCodes::fShader);

  Chimia::Rendering::IndexedBuffer buffer;
  buffer.Create(
    { Inputs::BufferData::vertex, Inputs::BufferData::vertexDataSize },
    { Inputs::BufferData::indexData, Inputs::BufferData::indexDataNItems },
    { Chimia::Rendering::ShaderAttribute::Float(0 /*position*/, 3 /*nFloats*/),
      Chimia::Rendering::ShaderAttribute::Float(1 /*color*/, 4 /*nFLoats*/) });

  Chimia::Rendering::IndexedBuffer buffer2;
  buffer2.Create(
    { Inputs::BufferData::vertex2, Inputs::BufferData::vertex2DataSize },
    { Inputs::BufferData::index2Data, Inputs::BufferData::index2DataNItems },
    { Chimia::Rendering::ShaderAttribute::Float(0 /*position*/, 3 /*nFloats*/),
      Chimia::Rendering::ShaderAttribute::Float(1 /*color*/, 4 /*nFLoats*/) });

  while (!win.ShouldClose()) {
    Chimia::Rendering::Clear(0.0f, 0.0f, 0.0f, 1.0f);
    shader.Use();
    buffer.Render();
    buffer2.Render();

    win.Swap();
    win.PollEvents();
  }

  buffer.Clear();
  shader.Clear();

  return 0;
}
