#include "Rendering/DataLayout.h"
#include "Rendering/RenderAction.h"
#include "Rendering/Rendering.h"

#include "Rendering/Shader.h"
#include "TestsUtils.h"
#include "Utils/Window.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>

#include <numeric>
#include <string>

namespace Scenarios {

void
Basic(Window& win)
{
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

  // clang-format off
  const float vertex[] = { 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
                          1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
                          0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };
  // clang-format on

  const unsigned vertexDataSize = 18 * sizeof(float);

  const unsigned indexData[] = { 0, 1, 2 };

  const unsigned indexDataNItems = 3;

  const Chimia::Rendering::DataLayout dataLayout{
    { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
    { "color", Chimia::Rendering::eDataType::VECTOR_3_FLOAT }
  };

  Chimia::Rendering::Shader shader;
  shader.Create(vShader, fShader, dataLayout);

  auto triangleVertexData = Chimia::Rendering::VertexData::New();
  triangleVertexData->Create({ vertex, vertexDataSize }, dataLayout);

  Chimia::Rendering::RenderAction renderTriangle;
  renderTriangle.Create(shader,
                        { { triangleVertexData, "pos", "pos" },
                          { triangleVertexData, "color", "color" } });

  shader.Use();
  renderTriangle.Render();

  win.Swap();

  TestsUtils::ExpectImage("test6_buffer_basic.png");
}

void
Indexed(Window& win)
{
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

  // clang-format off
  const float vertex[] = { 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
                          1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
                          0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };
  // clang-format on
  const unsigned nVertices = 3;

  const unsigned vertexDataSize = 18 * sizeof(float);

  const unsigned indexData[] = { 0, 1, 2 };

  const unsigned indexDataNItems = 3;

  const Chimia::Rendering::DataLayout dataLayout{
    { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
    { "color", Chimia::Rendering::eDataType::VECTOR_3_FLOAT }
  };

  Chimia::Rendering::Shader shader;
  shader.Create(vShader, fShader, dataLayout);

  auto triangleVertexData = Chimia::Rendering::VertexData::New();
  triangleVertexData->Create({ vertex, vertexDataSize }, dataLayout);

  auto triangleIndexData = Chimia::Rendering::IndexData::New();
  triangleIndexData->Create({ indexData, indexDataNItems });

  Chimia::Rendering::RenderAction renderTriangle;
  renderTriangle.Create(shader,
                        triangleIndexData,
                        { { triangleVertexData, "pos", "pos" },
                          { triangleVertexData, "color", "color" } });

  shader.Use();
  renderTriangle.Render();

  win.Swap();

  TestsUtils::ExpectImage("test6_buffer_indexed.png");
}

void
Subdata(Window& win)
{
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

  // clang-format off
  const std::vector<float> vertex = { 
                          // x     y     z     r     g     b
                          0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
                          1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
                          0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                          0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
                          -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
                          0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f};

  const std::vector<float> vertex2 = { 
                            // x     y     z     r     g     b
                            0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
                            -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
                            0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
                            0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
                            1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
                            0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
  // clang-format on

  const std::vector<std::vector<float>> vertexDatas{ vertex, vertex2 };

  const Chimia::Rendering::DataLayout dataLayout{
    { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
    { "color", Chimia::Rendering::eDataType::VECTOR_3_FLOAT }
  };

  Chimia::Rendering::Shader shader;
  shader.Create(vShader, fShader, dataLayout);

  auto vertexData = Chimia::Rendering::VertexData::New();
  vertexData->Create({ nullptr, vertex.size() * sizeof(float) }, dataLayout);

  Chimia::Rendering::RenderAction renderTriangle;
  renderTriangle.Create(
    shader, { { vertexData, "pos", "pos" }, { vertexData, "color", "color" } });

  const int EXTRA_STEPS = 2;
  int currentScreenshot = 1;
  for (int i = 0; i < vertexDatas.size() + EXTRA_STEPS; ++i) {
    Chimia::Rendering::Clear();

    const int current = i % vertexDatas.size();
    const std::vector<float>& data = vertexDatas[current];
    vertexData->Load(data);

    shader.Use();
    renderTriangle.Render();

    win.Swap();

    const std::string imageName = "test6_buffer_subdata_step" +
                                  std::to_string(currentScreenshot++) + ".png";
    TestsUtils::ExpectImage(imageName);
  }
}

void
SubDataWithVaryingSize(Window& win)
{
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

  // clang-format off
  const std::vector<float> vertex = { 
    // x     y     z     r     g     b
    0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
    -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
    0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f};

  const std::vector<float> vertex2 = { 
                          // x     y     z     r     g     b
                          0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
                          1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
                          0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                          0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
                          -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
                          0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f};

  const std::vector<float> vertex3 = { 
                            // x     y     z     r     g     b
                            0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
                            1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
                            0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                            0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
                            -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
                            0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                            0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 
                            -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 
                            -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};

  const std::vector<float> vertex4 = { 
                              // x     y     z     r     g     b
                              0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 
                              -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 
                              0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f};                           

  const std::vector<std::vector<float>> vertexStates = { vertex, vertex2, vertex3, vertex4};

  // clang-format on

  const Chimia::Rendering::DataLayout dataLayout{
    { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
    { "color", Chimia::Rendering::eDataType::VECTOR_3_FLOAT }
  };

  Chimia::Rendering::Shader shader;
  shader.Create(vShader, fShader, dataLayout);

  const auto& states = vertexStates;
  const size_t maximumSize =
    std::accumulate(states.begin(),
                    states.end(),
                    0,
                    [](size_t current, const std::vector<float>& incoming) {
                      return std::max(current, incoming.size());
                    });

  auto vertexData = Chimia::Rendering::VertexData::New();
  vertexData->Create({ nullptr, maximumSize * sizeof(float) }, dataLayout);

  Chimia::Rendering::RenderAction renderTriangles;
  renderTriangles.Create(
    shader, { { vertexData, "pos", "pos" }, { vertexData, "color", "color" } });

  constexpr int EXTRA_STEPS = 2;
  int currentScreenshot = 1;
  for (int i = 0; i < states.size() + EXTRA_STEPS; ++i) {
    Chimia::Rendering::Clear();

    const int current = i % states.size();
    const std::vector<float>& data = states[current];

    vertexData->Load(data);

    shader.Use();
    renderTriangles.Render();

    win.Swap();

    const std::string imageName = "test6_buffer_subdata_varying_step" +
                                  std::to_string(currentScreenshot++) + ".png";
    TestsUtils::ExpectImage(imageName);
  }
}

void
VertexAndIndexSubData(Window& win)
{
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

  using VertexData = std::vector<float>;
  using IndexData = std::vector<unsigned>;

  // clang-format off
  const VertexData vertex = { 
    // x     y     z     r     g     b
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 
    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 
    0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f };

  const VertexData vertex2 = { 
      // x     y     z     r     g     b
      -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 
      0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 
      0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f };
    


  const IndexData indices = {
    0, 1, 2
  };
  const IndexData indices2 = {
    0, 1, 3
  };

  const IndexData indices3 = {
    0, 1, 2,
    2, 3, 0
  };

  using State = struct {
    const VertexData& vData;
    const IndexData& iData; 
  };

  const std::vector<State> states = {
    {vertex, indices},
    {vertex, indices2},
    {vertex, indices3},
    {vertex2, indices}
  };

  // clang-format on

  const Chimia::Rendering::DataLayout dataLayout{
    { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
    { "color", Chimia::Rendering::eDataType::VECTOR_3_FLOAT }
  };

  Chimia::Rendering::Shader shader;
  shader.Create(vShader, fShader, dataLayout);

  const size_t maximumVertexSize = std::accumulate(
    states.begin(), states.end(), 0, [](size_t current, const auto& incoming) {
      return std::max(current, incoming.vData.size());
    });
  const size_t maximumIndexSize = std::accumulate(
    states.begin(), states.end(), 0, [](size_t current, const auto& incoming) {
      return std::max(current, incoming.iData.size());
    });

  auto vertexData = Chimia::Rendering::VertexData::New();
  vertexData->Create({ nullptr, maximumVertexSize * sizeof(float) },
                     dataLayout);

  auto indexData = Chimia::Rendering::IndexData::New();
  indexData->Create({ nullptr, maximumIndexSize, sizeof(unsigned) });

  Chimia::Rendering::RenderAction renderTriangles;
  renderTriangles.Create(
    shader,
    indexData,
    { { vertexData, "pos", "pos" }, { vertexData, "color", "color" } });

  constexpr int EXTRA_STEPS = 2;
  int currentScreenshot = 1;
  for (int i = 0; i < states.size() + EXTRA_STEPS; ++i) {

    Chimia::Rendering::Clear();

    const int current = i % states.size();
    const auto& state = states[current];

    vertexData->Load(state.vData);
    indexData->LoadIndexData(state.iData);

    shader.Use();
    renderTriangles.Render();

    win.Swap();

    const std::string imageName = "test6_buffer_subdata_vertex_and_index_step" +
                                  std::to_string(currentScreenshot++) + ".png";
    TestsUtils::ExpectImage(imageName);
  }
}

}

int
main(int argc, char** argv)
{
  constexpr unsigned SCR_WIDTH = 800;
  constexpr unsigned SCR_HEIGHT = 600;
  Window win(SCR_WIDTH, SCR_HEIGHT, "Test #6");

  TestsUtils::InitTesting(argv, win);
  Chimia::Rendering::Initialize();

  Scenarios::Basic(win);
  Scenarios::Indexed(win);
  Scenarios::Subdata(win);
  Scenarios::SubDataWithVaryingSize(win);
  Scenarios::VertexAndIndexSubData(win);

  return 0;
}