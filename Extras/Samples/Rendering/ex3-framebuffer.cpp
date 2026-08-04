#include "Rendering/DataLayout.h"
#include "Rendering/RenderAction.h"
#include "Rendering/Rendering.h"

#include "Media/Image.h"

#include "Rendering/FrameBuffer.h"
#include "Rendering/Shader.h"
#include "Rendering/Texture2D.h"
#include "Rendering/TextureUnit.h"

#include "Utils/ExtrasUtils.h"
#include "Utils/Window.h"

namespace Inputs {

constexpr unsigned SCR_WIDTH = 800;
constexpr unsigned SCR_HEIGHT = 600;

namespace ShaderCodes {

const char* vShader = R"(
      #version 330

      layout (location = 0) in vec3 pos;
      layout (location = 1) in vec2 uv;

      out vec2 textureUV;

      void main() {
        gl_Position = vec4(pos, 1.0);
        textureUV = uv;
      }
    )";

const char* fShader = R"(
      #version 330

      in vec2 textureUV;

      out vec4 fragColor;

      uniform sampler2D tex;

      void main() {
        fragColor = texture(tex, textureUV);
        //fragColor = vec4(1.0, 0.5, 0.5, 1.0);
      }
    )";

const char* vShaderPost = R"(
      #version 330

      layout (location = 0) in vec3 pos;
      layout (location = 1) in vec2 uv;

      out vec2 textureUV;

      void main() {
        gl_Position = vec4(pos, 1.0);
        textureUV = uv;
      }
    )";

const char* fShaderPost = R"(
      #version 330

      in vec2 textureUV;

      out vec4 fragColor;

      uniform sampler2D tex;

      void main() {
        const float offset = 0.01;

        vec2 uvXplus = vec2(textureUV.x + offset, textureUV.y);
        vec2 uvXminus = vec2(textureUV.x - offset, textureUV.y);
        vec2 uvYplus = vec2(textureUV.x, textureUV.y + offset);
        vec2 uvYminus = vec2(textureUV.x, textureUV.y - offset);

        fragColor = 0.4 * texture(tex, textureUV) + 
                    0.15 * texture(tex, uvXplus) + 
                    0.15 * texture(tex, uvXminus) + 
                    0.15 * texture(tex, uvYplus) + 
                    0.15 * texture(tex, uvYminus);

        //fragColor = texture(tex, textureUV);
      }
    )";
}

namespace BufferData {

// clang-format off
const std::vector<float> vertex{
// x     y    z     u     v
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 
  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
};
// clang-format on

const std::vector<unsigned> index{ 0, 1, 2 };

// clang-format off
const std::vector<float> quad{ // x    y     z     u     v
                               -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 
                               1.0f, -1.0f, 0.0f,  1.0f,  0.0f, 
                               1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 
                               -1.0f, 1.0f, 0.0f, 0.0f, 1.0f
};
// clang-format on

const std::vector<unsigned> quadIndex{ 0, 1, 2, 2, 3, 0 };

}

}

int
main(int argc, char** argv)
{
  Window win(Inputs::SCR_WIDTH, Inputs::SCR_HEIGHT, "Example #3");

  Chimia::Rendering::Initialize();

  Chimia::Rendering::SetViewport(0, 0, Inputs::SCR_WIDTH, Inputs::SCR_HEIGHT);

  const Chimia::Rendering::DataLayout dataLayout{
    { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
    { "uv", Chimia::Rendering::eDataType::VECTOR_2_FLOAT }
  };

  Chimia::Rendering::Shader shader;
  shader.Create(
    Inputs::ShaderCodes::vShader, Inputs::ShaderCodes::fShader, dataLayout);

  auto vertexData = Chimia::Rendering::VertexData::New();
  vertexData->Create(Inputs::BufferData::vertex, dataLayout);

  auto indexData = Chimia::Rendering::IndexData::New();
  indexData->Create(Inputs::BufferData::index);

  Chimia::Rendering::RenderAction action;
  action.Create(shader,
                indexData,
                { { vertexData, "pos", "pos" }, { vertexData, "uv", "uv" } });

  const std::string assetsDir =
    ExtrasUtils::GetCurrentAppDir(argv) + "/assets/";
  const std::string solarFlareAsset = assetsDir + "solar-flare.jpg";
  Chimia::Media::Image texData(solarFlareAsset.c_str());

  Chimia::Rendering::Texture2D texture(
    texData.RawData(), texData.Width(), texData.Height());

  const Chimia::Rendering::TextureUnit texUnit =
    Chimia::Rendering::TextureUnit::UNIT_1;

  Chimia::Rendering::FrameBuffer frameBuffer(Inputs::SCR_WIDTH,
                                             Inputs::SCR_HEIGHT);

  Chimia::Rendering::Shader secondPassShader;
  secondPassShader.Create(Inputs::ShaderCodes::vShaderPost,
                          Inputs::ShaderCodes::fShaderPost,
                          dataLayout);

  auto quadVertexData = Chimia::Rendering::VertexData::New();
  quadVertexData->Create(Inputs::BufferData::quad, dataLayout);

  auto quadIndexData = Chimia::Rendering::IndexData::New();
  quadIndexData->Create(Inputs::BufferData::quadIndex);

  Chimia::Rendering::RenderAction renderScreenQuadAction;
  renderScreenQuadAction.Create(
    shader,
    quadIndexData,
    { { quadVertexData, "pos", "pos" }, { quadVertexData, "uv", "uv" } });

  const Chimia::Rendering::TextureUnit texUnitPost =
    Chimia::Rendering::TextureUnit::UNIT_2;

  while (!win.ShouldClose()) {
    frameBuffer.Use();

    shader.Use();
    shader.SetTexture("tex", texture, texUnit);
    action.Render();

    Chimia::Rendering::FrameBuffer::UseDefaultFrameBuffer();

    secondPassShader.Use();
    secondPassShader.SetTexture("tex", frameBuffer.GetTexture(), texUnitPost);
    renderScreenQuadAction.Render();

    win.Swap();
    win.PollEvents();
  }

  return 0;
}