#include "Rendering/DataLayout.h"
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

// 1) Opaque triangle

constexpr float opaqueDepth = 0.0f;
// clang-format off
const float vertex[] = { 0.0f, 0.0f, opaqueDepth, 0.0f, 1.0f, 0.0f, 1.0f,
                         1.0f, 0.0f, opaqueDepth, 1.0f, 0.0f, 0.0f, 1.0f,
                        0.0f, 1.0f, opaqueDepth, 0.0f, 0.0f, 1.0f, 1.0f };
// clang-format on

const unsigned vertexDataSize = 21 * sizeof(float);

const unsigned indexData[] = { 0, 1, 2 };

const unsigned indexDataNItems = 3;

// 2) Transparent triangle #1

constexpr float alpha1 = 0.3f;

constexpr float transparentDepth1 = -0.5f;
// clang-format off
const float vertex2[] = { -1.0f, 0.0f, transparentDepth1, 0.0f, 1.0f, 0.0f, alpha1,
                         1.0f, 0.0f, transparentDepth1, 0.0f, 1.0f, 0.0f, alpha1,
                        1.0f, 0.5f, transparentDepth1, 0.0f, 1.0f, 0.0f, alpha1 };
// clang-format on

const unsigned vertex2DataSize = 21 * sizeof(float);

// 3) Transparent triangle #2

constexpr float alpha2 = 0.3f;

constexpr float transparentDepth2 = 0.75f;

// clang-format off
const float vertex3[] = { -1.0f, 0.0f, transparentDepth2, 0.0f, 0.0f, 1.0f, alpha2,
                         1.0f, 0.0f, transparentDepth2, 0.0f, 0.0f, 1.0f, alpha2,
                        -1.0f, 1.0f, transparentDepth2, 0.0f, 0.0f, 1.0f, alpha2 };
// clang-format on

const unsigned vertex3DataSize = 21 * sizeof(float);
}
}

int
main()
{
  Window win(1280, 1080, "Example #14");

  Chimia::Rendering::Initialize();
  Chimia::Rendering::EnableDepthTest(true);
  Chimia::Rendering::EnableColorBlend(true);

  const Chimia::Rendering::DataLayout dataLayout{
    { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
    { "color", Chimia::Rendering::eDataType::VECTOR_4_FLOAT }
  };

  auto shader = Chimia::Rendering::Shader::Create(
    Inputs::ShaderCodes::vShader, Inputs::ShaderCodes::fShader, dataLayout);

  auto vertexData1 = Chimia::Rendering::VertexData::Create(
    { Inputs::BufferData::vertex, Inputs::BufferData::vertexDataSize },
    dataLayout);

  auto vertexData2 = Chimia::Rendering::VertexData::Create(
    { Inputs::BufferData::vertex2, Inputs::BufferData::vertex2DataSize },
    dataLayout);

  auto vertexData3 = Chimia::Rendering::VertexData::Create(
    { Inputs::BufferData::vertex3, Inputs::BufferData::vertex3DataSize },
    dataLayout);

  auto indexData = Chimia::Rendering::IndexData::Create(
    { Inputs::BufferData::indexData, Inputs::BufferData::indexDataNItems });

  auto target = Chimia::Rendering::Target::Create(shader);

  Chimia::Rendering::RenderAction opaqueTriangle;
  opaqueTriangle.Create(
    target,
    indexData,
    { { vertexData1, "pos", "pos" }, { vertexData1, "color", "color" } },
    Chimia::Rendering::ePrimitive::TRIANGLES);

  Chimia::Rendering::RenderAction transparentTriangle1;
  transparentTriangle1.Create(
    target,
    indexData,
    { { vertexData2, "pos", "pos" }, { vertexData2, "color", "color" } },
    Chimia::Rendering::ePrimitive::TRIANGLES);

  Chimia::Rendering::RenderAction transparentTriangle2;
  transparentTriangle2.Create(
    target,
    indexData,
    { { vertexData3, "pos", "pos" }, { vertexData3, "color", "color" } },
    Chimia::Rendering::ePrimitive::TRIANGLES);

  while (!win.ShouldClose()) {
    Chimia::Rendering::EnableDepthMask(true);

    Chimia::Rendering::Clear(0.0f, 0.0f, 0.0f, 1.0f);

    opaqueTriangle.Render();

    Chimia::Rendering::EnableDepthMask(false);
    transparentTriangle1.Render();
    transparentTriangle2.Render();

    win.Swap();
    win.PollEvents();
  }

  return 0;
}
