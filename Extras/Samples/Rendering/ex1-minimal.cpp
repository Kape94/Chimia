#include "Rendering/DataLayout.h"
#include "Rendering/IndexData.h"
#include "Rendering/RenderAction.h"
#include "Rendering/Rendering.h"

#include "Rendering/Shader.h"

#include "Rendering/Target.h"
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

const unsigned indexData[] = { 0, 1, 2 };

const unsigned indexDataNItems = 3;
}
}

int
main()
{
  Window win(1280, 1080, "Example #1");

  Chimia::Rendering::Initialize();

  auto shader = Chimia::Rendering::Shader::Create(
    Inputs::ShaderCodes::vShader,
    Inputs::ShaderCodes::fShader,
    { { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
      { "color", Chimia::Rendering::eDataType::VECTOR_3_FLOAT } });

  auto vertexData = Chimia::Rendering::VertexData::Create(
    { Inputs::BufferData::vertex, Inputs::BufferData::vertexDataSize },
    { { "position", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
      { "color", Chimia::Rendering::eDataType::VECTOR_3_FLOAT } });

  auto indexData = Chimia::Rendering::IndexData::Create(
    { Inputs::BufferData::indexData, Inputs::BufferData::indexDataNItems });

  auto target = Chimia::Rendering::Target::Create(shader);

  Chimia::Rendering::RenderAction action;
  action.Create(
    target,
    indexData,
    { { vertexData, "position", "pos" }, { vertexData, "color", "color" } });

  while (!win.ShouldClose()) {
    action.Render();

    win.Swap();
    win.PollEvents();
  }

  return 0;
}
