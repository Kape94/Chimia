#include "Rendering/DataLayout.h"
#include "Rendering/GenericRenderAction.h"
#include "Rendering/Rendering.h"

#include "Rendering/Shader.h"

#include "Rendering/ShaderBinding.h"
#include "Rendering/VertexData.h"
#include "Utils/Window.h"

#include "TestsUtils.h"

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

// clang-format off
const float vertex[] = { 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
                         1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
                         0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };
// clang-format on

const unsigned vertexDataSize = 18 * sizeof(float);

const unsigned indexData[] = { 0, 1, 2 };

const unsigned indexDataNItems = 3;
}
}

int
main(int argc, char** argv)
{
  Window win(800, 600, "Rendering test #1");

  TestsUtils::InitTesting(argv, win);

  Chimia::Rendering::Initialize();

  const Chimia::Rendering::DataLayout dataLayout{
    { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
    { "color", Chimia::Rendering::eDataType::VECTOR_3_FLOAT }
  };

  Chimia::Rendering::Shader shader;
  shader.Create(
    Inputs::ShaderCodes::vShader, Inputs::ShaderCodes::fShader, dataLayout);

  auto vertexData = Chimia::Rendering::VertexData::New();
  vertexData->Create(
    { Inputs::BufferData::vertex, Inputs::BufferData::vertexDataSize },
    dataLayout);

  auto indexData = Chimia::Rendering::IndexData::New();
  indexData->Create(
    { Inputs::BufferData::indexData, Inputs::BufferData::indexDataNItems });

  Chimia::Rendering::GenericRenderAction action;
  action.Create({ Chimia::Rendering::ShaderBinding::Connect(
                    vertexData, "pos", shader, "pos"),
                  Chimia::Rendering::ShaderBinding::Connect(
                    vertexData, "color", shader, "color") },
                indexData);

  shader.Use();
  action.Render();

  win.Swap();

  TestsUtils::ExpectImage("test1_basic.png");

  return 0;
}
