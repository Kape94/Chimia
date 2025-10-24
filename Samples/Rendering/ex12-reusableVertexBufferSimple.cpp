#include "Rendering/Rendering.h"

#include "Rendering/IndexedBuffer.h"
#include "Rendering/ReusableVertexBufferObject.h"
#include "Rendering/Shader.h"

#include "Rendering/ShaderAttribute.h"
#include "Utils/Window.h"

namespace Inputs {

namespace ShaderCodes {

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

}

namespace BufferData {

const float vertex[] = { 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                         1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };

const unsigned vertexDataSize = 18 * sizeof(float);

const unsigned indexData[] = { 0, 1, 2 };

const unsigned indexDataSize = 3;
}
}

int
main()
{
  Window win(1280, 1080, "Example #12");

  Chimia::Rendering::Initialize();

  Chimia::Rendering::Shader shader;
  shader.Create(Inputs::ShaderCodes::vShader, Inputs::ShaderCodes::fShader);

  const Chimia::Rendering::ShaderAttributes vertexAttributes{
    Chimia::Rendering::ShaderAttribute::Float(0 /*position*/, 3 /*nFloats*/),
    Chimia::Rendering::ShaderAttribute::Float(1 /*color*/, 3 /*nFLoats*/)
  };

  Chimia::Rendering::ReusableVertexBufferObject reusableVertexBuffer;
  reusableVertexBuffer.Create(Inputs::BufferData::vertex,
                              Inputs::BufferData::vertexDataSize,
                              Inputs::BufferData::indexData,
                              Inputs::BufferData::indexDataSize,
                              vertexAttributes);

  Chimia::Rendering::IndexedBuffer buffer;
  buffer.Create(reusableVertexBuffer);

  while (!win.ShouldClose()) {
    shader.Use();
    buffer.Render();

    win.Swap();
    win.PollEvents();
  }

  buffer.Clear();
  shader.Clear();

  return 0;
}
