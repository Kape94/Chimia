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
      layout (location = 1) in vec3 color;

      out vec3 vertexColor;

      void main() {
        gl_Position = vec4(pos, 1.0);
        gl_PointSize = 25.0;

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
const std::vector<float> vertex{ 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
                                 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
                                 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
                                 0.5f, 0.5f, 0.0f, 0.5f, 1.0f, 1.0f };
// clang-format on

const std::vector<unsigned> triangleIndexData{ 0, 1, 2 };
const std::vector<unsigned> linesIndexData{ 0, 1, 0, 3, 3, 1 };
const std::vector<unsigned> pointsIndexData{ 0, 1, 2, 3 };

}
}

int
main()
{
  Window win(1280, 1080, "Example #18");

  Chimia::Rendering::Initialize();

  auto shader = Chimia::Rendering::Shader::Create(
    Inputs::ShaderCodes::vShader,
    Inputs::ShaderCodes::fShader,
    { { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
      { "color", Chimia::Rendering::eDataType::VECTOR_3_FLOAT } });

  auto vertexData = Chimia::Rendering::VertexData::Create(
    Inputs::BufferData::vertex,
    { { "position", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
      { "color", Chimia::Rendering::eDataType::VECTOR_3_FLOAT } });

  auto triangleIndexData =
    Chimia::Rendering::IndexData::Create(Inputs::BufferData::triangleIndexData);
  auto linesIndexData =
    Chimia::Rendering::IndexData::Create(Inputs::BufferData::linesIndexData);
  auto pointsIndexData =
    Chimia::Rendering::IndexData::Create(Inputs::BufferData::pointsIndexData);

  auto target = Chimia::Rendering::Target::Create(shader);

  Chimia::Rendering::RenderAction drawTriangle;
  drawTriangle.Create(
    target,
    triangleIndexData,
    { { vertexData, "position", "pos" }, { vertexData, "color", "color" } },
    Chimia::Rendering::ePrimitive::TRIANGLES);

  Chimia::Rendering::RenderAction drawLines;
  drawLines.Create(
    target,
    linesIndexData,
    { { vertexData, "position", "pos" }, { vertexData, "color", "color" } },
    Chimia::Rendering::ePrimitive::LINES);

  Chimia::Rendering::RenderAction drawPoints;
  drawPoints.Create(
    target,
    pointsIndexData,
    { { vertexData, "position", "pos" }, { vertexData, "color", "color" } },
    Chimia::Rendering::ePrimitive::POINTS);

  const std::vector<const Chimia::Rendering::RenderAction*> drawActions{
    &drawPoints, &drawLines, &drawTriangle
  };
  const size_t nDrawActions = drawActions.size();

  size_t selected = 0;
  unsigned count = 0;
  const unsigned flipCount = 400;

  while (!win.ShouldClose()) {
    Chimia::Rendering::Clear();

    const Chimia::Rendering::RenderAction* drawAction = drawActions[selected];
    drawAction->Render();

    if (count >= flipCount) {
      selected = (selected + 1) % nDrawActions;
      count = 0;
    }
    ++count;

    win.Swap();
    win.PollEvents();
  }

  return 0;
}
