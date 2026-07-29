#include "Rendering/DataLayout.h"
#include "Rendering/GenericRenderAction.h"
#include "Rendering/Rendering.h"

#include "Rendering/Shader.h"
#include "Rendering/ShaderBinding.h"
#include "TestsUtils.h"
#include "Utils/Window.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>

#include <string>

namespace Scenarios {

void
BlendingSimple(Window& win)
{
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

  const unsigned index2Data[] = { 0, 1, 2 };

  const unsigned index2DataNItems = 3;

  // 3) Transparent triangle #2

  constexpr float alpha2 = 0.3f;

  constexpr float transparentDepth2 = 0.75f;

  // clang-format off
  const float vertex3[] = { -1.0f, 0.0f, transparentDepth2, 0.0f, 0.0f, 1.0f, alpha2,
                          1.0f, 0.0f, transparentDepth2, 0.0f, 0.0f, 1.0f, alpha2,
                          -1.0f, 1.0f, transparentDepth2, 0.0f, 0.0f, 1.0f, alpha2 };
  // clang-format on

  const unsigned vertex3DataSize = 21 * sizeof(float);

  const unsigned index3Data[] = { 0, 1, 2 };

  const unsigned index3DataNItems = 3;

  const unsigned nVertices = 3;

  Chimia::Rendering::EnableDepthTest(true);
  Chimia::Rendering::EnableColorBlend(true);

  const Chimia::Rendering::DataLayout dataLayout{
    { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
    { "color", Chimia::Rendering::eDataType::VECTOR_4_FLOAT },
  };

  Chimia::Rendering::Shader shader;
  shader.Create(vShader, fShader, dataLayout);

  auto vertexData1 = Chimia::Rendering::VertexData::New();
  vertexData1->Create({ vertex, vertexDataSize }, dataLayout);

  auto vertexData2 = Chimia::Rendering::VertexData::New();
  vertexData2->Create({ vertex2, vertex2DataSize }, dataLayout);

  auto vertexData3 = Chimia::Rendering::VertexData::New();
  vertexData3->Create({ vertex3, vertex3DataSize }, dataLayout);

  auto triangleIndexData = Chimia::Rendering::IndexData::New();
  triangleIndexData->Create({ indexData, indexDataNItems });

  Chimia::Rendering::GenericRenderAction opaqueTriangle;
  opaqueTriangle.Create({ Chimia::Rendering::ShaderBinding::Connect(
                            vertexData1, "pos", shader, "pos"),
                          Chimia::Rendering::ShaderBinding::Connect(
                            vertexData1, "color", shader, "color") },
                        triangleIndexData);

  Chimia::Rendering::GenericRenderAction transparentTriangle1;
  transparentTriangle1.Create({ Chimia::Rendering::ShaderBinding::Connect(
                                  vertexData2, "pos", shader, "pos"),
                                Chimia::Rendering::ShaderBinding::Connect(
                                  vertexData2, "color", shader, "color") },
                              triangleIndexData);

  Chimia::Rendering::GenericRenderAction transparentTriangle2;
  transparentTriangle2.Create({ Chimia::Rendering::ShaderBinding::Connect(
                                  vertexData3, "pos", shader, "pos"),
                                Chimia::Rendering::ShaderBinding::Connect(
                                  vertexData3, "color", shader, "color") },
                              triangleIndexData);

  Chimia::Rendering::EnableDepthMask(true);

  Chimia::Rendering::Clear(0.0f, 0.0f, 0.0f, 1.0f);
  shader.Use();
  opaqueTriangle.Render();

  Chimia::Rendering::EnableDepthMask(false);
  transparentTriangle1.Render();
  transparentTriangle2.Render();

  win.Swap();

  TestsUtils::ExpectImage("test8_colorBlending.png");
}

}

int
main(int argc, char** argv)
{
  constexpr unsigned SCR_WIDTH = 800;
  constexpr unsigned SCR_HEIGHT = 600;
  Window win(SCR_WIDTH, SCR_HEIGHT, "Test #8");

  TestsUtils::InitTesting(argv, win);
  Chimia::Rendering::Initialize();

  Scenarios::BlendingSimple(win);

  return 0;
}