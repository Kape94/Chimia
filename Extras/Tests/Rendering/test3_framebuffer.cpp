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

#include "TestsUtils.h"

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
  Window win(Inputs::SCR_WIDTH, Inputs::SCR_HEIGHT, "Test #3 - Rendering");

  const auto [scrWidth, scrHeight] = win.GetFramebufferSize();

  TestsUtils::InitTesting(argv, win);

  Chimia::Rendering::Initialize();

  Chimia::Rendering::SetViewport(0, 0, scrWidth, scrHeight);

  const Chimia::Rendering::DataLayout datalayout{
    { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
    { "uv", Chimia::Rendering::eDataType::VECTOR_2_FLOAT }
  };

  Chimia::Rendering::Shader shader;
  shader.Create(
    Inputs::ShaderCodes::vShader, Inputs::ShaderCodes::fShader, datalayout);

  Chimia::Rendering::FrameBuffer frameBuffer(scrWidth, scrHeight);

  auto vertexData = Chimia::Rendering::VertexData::New();
  vertexData->Create(Inputs::BufferData::vertex, datalayout);

  auto indexData = Chimia::Rendering::IndexData::New();
  indexData->Create(Inputs::BufferData::index);

  auto renderToFramebuffer =
    Chimia::Rendering::Target::Create(shader, frameBuffer);

  Chimia::Rendering::RenderAction renderTriangle;
  renderTriangle.Create(
    renderToFramebuffer,
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

  Chimia::Rendering::Shader secondPassShader;
  secondPassShader.Create(Inputs::ShaderCodes::vShaderPost,
                          Inputs::ShaderCodes::fShaderPost,
                          datalayout);

  auto vertexDataQuad = Chimia::Rendering::VertexData::New();
  vertexDataQuad->Create(Inputs::BufferData::quad, datalayout);

  auto indexDataQuad = Chimia::Rendering::IndexData::New();
  indexDataQuad->Create(Inputs::BufferData::quadIndex);

  auto renderToScreen = Chimia::Rendering::Target::Create(secondPassShader);

  Chimia::Rendering::RenderAction renderScreenQuad;
  renderScreenQuad.Create(
    renderToScreen,
    indexDataQuad,
    { { vertexDataQuad, "pos", "pos" }, { vertexDataQuad, "uv", "uv" } });

  const Chimia::Rendering::TextureUnit texUnitPost =
    Chimia::Rendering::TextureUnit::UNIT_2;

  shader.SetTexture("tex", texture, texUnit);
  renderTriangle.Render();

  secondPassShader.SetTexture("tex", frameBuffer.GetTexture(), texUnitPost);
  renderScreenQuad.Render();

  win.Swap();

  TestsUtils::ExpectImage("test3_framebuffer.png");

  return 0;
}