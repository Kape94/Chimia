#include "Rendering/Rendering.h"

#include "Rendering/IndexedBuffer.h"
#include "Rendering/ReusableIndexedVertexBufferObject.h"
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
const unsigned vertexDataItems = 18;
const unsigned vertexDataSize = vertexDataItems * sizeof(float);
const unsigned nVertices = 3;

const unsigned indexData[] = { 0, 1, 2 };

const unsigned indexDataItems = 3;
}
}

int
main()
{
  Window win(1280, 1080, "Example #12");

  Chimia::Rendering::Initialize();

  Chimia::Rendering::Shader shader;
  shader.Create(Inputs::ShaderCodes::vShader, Inputs::ShaderCodes::fShader);

  Chimia::Rendering::ReusableIndexedVertexBufferObject reusableVertexBuffer;
  reusableVertexBuffer.Create(
    { Inputs::BufferData::vertex, Inputs::BufferData::vertexDataSize },
    Inputs::BufferData::nVertices,
    { Inputs::BufferData::indexData,
      Inputs::BufferData::indexDataItems,
      sizeof(unsigned) });

  Chimia::Rendering::IndexedRenderAction action;
  action.Create(
    reusableVertexBuffer,
    { Chimia::Rendering::ShaderAttribute::Float(0 /*position*/, 3 /*nFloats*/),
      Chimia::Rendering::ShaderAttribute::Float(1 /*color*/, 3 /*nFLoats*/) });

  while (!win.ShouldClose()) {
    shader.Use();
    action.Render();

    win.Swap();
    win.PollEvents();
  }

  action.Clear();
  shader.Clear();

  return 0;
}
