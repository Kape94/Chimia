#include "Rendering/Rendering.h"

#include "Rendering/InstancedBuffer.h"
#include "Rendering/Shader.h"

#include "Utils/ExtrasUtils.h"
#include "Utils/Window.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <vector>

namespace Inputs {

constexpr unsigned SCR_WIDTH = 800;
constexpr unsigned SCR_HEIGHT = 600;

namespace ShaderCodes {

const char* vShader = R"(
      #version 330

      layout (location = 0) in vec3 pos;
      layout (location = 1) in vec2 offset;

      void main() {
        gl_Position = vec4(pos, 1.0) + vec4(offset, 0.0, 0.0);
      }
    )";

const char* fShader = R"(
      #version 330

      out vec4 fragColor;

      void main() {
        fragColor = vec4(1.0, 0.5, 0.5, 1.0);
      }
    )";
}

namespace BufferData {

// clang-format off
const std::vector<float> vertex{ // x ,    y,    z
                                 0.0f,  0.0f, 0.0f, 
                                 0.1f, 0.0f,  0.0f,
                                 0.0f,  0.1f, 0.0f, 
                                 0.0f, 0.0f,  0.0f,
                                 -0.1f, 0.0f, 0.0f, 
                                 0.0f, -0.1f, 0.0f
};
// clang-format on

}

namespace InstanceData {

const unsigned dataSize = sizeof(glm::vec2);

const std::vector<glm::vec2> positions{
  { 0.2, 0.2 }, { -0.2, 0.2 }, { -0.2, -0.2 }, { 0.2, -0.2 }, { 0.3, 0.3 },
};

const std::vector<glm::vec2> positions2{
  { 0.4, 0.4 }, { -0.4, 0.4 }, { -0.4, -0.4 }, { 0.4, -0.4 }, { 0.3, 0.3 },
};

const std::vector<glm::vec2> positions3{
  { 0.3, 0.3 }, { -0.3, 0.3 }, { -0.3, -0.3 }, { 0.3, -0.3 }, { 0.3, 0.3 },
};

const std::vector<glm::vec2> positions4{
  { 0.5, 0.5 },
  { -0.5, 0.5 },
};

const std::vector<glm::vec2> positions5{ { 0.7, 0.7 } };

const std::vector<glm::vec2> positions6{ { 0.1, 0.1 },
                                         { 0.1, 0.7 },
                                         { 0.5, 0.5 } };

const std::vector<std::vector<glm::vec2>> positionGroups{
  positions, positions2, positions3, positions4, positions5, positions6
};

}

}

int
main()
{
  Window win(Inputs::SCR_WIDTH, Inputs::SCR_HEIGHT, "Example #8");

  Chimia::Rendering::Initialize();
  Chimia::Rendering::SetViewport(0, 0, Inputs::SCR_WIDTH, Inputs::SCR_HEIGHT);

  Chimia::Rendering::Shader shader(Inputs::ShaderCodes::vShader,
                                   Inputs::ShaderCodes::fShader);

  Chimia::Rendering::InstancedRenderAction action;
  action.CreateInstanced(Inputs::BufferData::vertex,
                         { Chimia::Rendering::ShaderAttribute::Float(
                           0 /*location*/, 3 /*nEntries*/) },
                         { nullptr,
                           Inputs::InstanceData::positions.size(),
                           Inputs::InstanceData::dataSize },
                         { Chimia::Rendering::ShaderAttribute::Float(
                           1 /*location*/, 2 /*nEntries*/) });

  action.LoadInstancedData(Inputs::InstanceData::positions);

  int selectedGroup = 0;
  while (!win.ShouldClose()) {
    Chimia::Rendering::Clear();

    const std::vector<glm::vec2>& positions =
      Inputs::InstanceData::positionGroups[selectedGroup];
    action.LoadInstancedData(positions);

    shader.Use();
    action.Render();

    win.Swap();
    win.PollEvents();

    selectedGroup =
      (selectedGroup + 1) % Inputs::InstanceData::positionGroups.size();
    ExtrasUtils::SyncForTargetFPS(1);
  }

  return 0;
}