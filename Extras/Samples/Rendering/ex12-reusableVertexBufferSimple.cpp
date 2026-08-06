#include "Rendering/IndexData.h"
#include "Rendering/RenderAction.h"
#include "Rendering/Rendering.h"

#include "Rendering/Shader.h"

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

const unsigned indexData[] = { 0, 1, 2 };

const unsigned indexDataItems = 3;
}
}

int
main()
{
  Window win(1280, 1080, "Example #12");

  Chimia::Rendering::Initialize();

  const Chimia::Rendering::DataLayout dataLayout{
    { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
    { "color", Chimia::Rendering::eDataType::VECTOR_3_FLOAT }
  };

  auto shader = Chimia::Rendering::Shader::Create(
    Inputs::ShaderCodes::vShader, Inputs::ShaderCodes::fShader, dataLayout);

  auto reusableVertexData = Chimia::Rendering::VertexData::New();
  reusableVertexData->Create(
    { Inputs::BufferData::vertex, Inputs::BufferData::vertexDataSize },
    dataLayout);

  auto reusableIndexData = Chimia::Rendering::IndexData::New();
  reusableIndexData->Create({ Inputs::BufferData::indexData,
                              Inputs::BufferData::indexDataItems,
                              sizeof(unsigned) });

  auto target = Chimia::Rendering::Target::Create(shader);

  Chimia::Rendering::RenderAction action;
  action.Create(target,
                reusableIndexData,
                { { reusableVertexData, "pos", "pos" },
                  { reusableVertexData, "color", "color" } });

  while (!win.ShouldClose()) {
    action.Render();

    win.Swap();
    win.PollEvents();
  }

  return 0;
}
