#include "Rendering/IndexData.h"
#include "Rendering/Rendering.h"

#include "Rendering/IndexedRenderAction.h"
#include "Rendering/Shader.h"

#include "Rendering/VertexData.h"
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
const unsigned nVertices = 3;

const unsigned indexData[] = { 0, 1, 2 };

const unsigned indexDataNItems = 3;
}
}

int
main()
{
  Window win(1280, 1080, "Example #1");

  Chimia::Rendering::Initialize();

  Chimia::Rendering::Shader shader;
  shader.Create(Inputs::ShaderCodes::vShader, Inputs::ShaderCodes::fShader);

  auto vertexData = Chimia::Rendering::VertexData::New();

  vertexData->Create(
    { Inputs::BufferData::vertex, Inputs::BufferData::vertexDataSize },
    Inputs::BufferData::nVertices);

  auto indexData = Chimia::Rendering::IndexData::New();
  indexData->Create(
    { Inputs::BufferData::indexData, Inputs::BufferData::indexDataNItems });

  Chimia::Rendering::IndexedRenderAction action;
  action.Create(
    vertexData,
    indexData,
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
