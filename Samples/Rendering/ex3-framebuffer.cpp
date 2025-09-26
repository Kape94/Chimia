#include "Rendering/Rendering.h"

#include "Rendering/FrameBuffer.h"
#include "Rendering/IndexedBuffer.h"
#include "Rendering/Shader.h"
#include "Rendering/Texture2D.h"
#include "Rendering/TextureUnit.h"

#include "Utils/SamplesUtils.h"
#include "Utils/Window.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Utils/stb_image.h"

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

  Chimia::Rendering::Shader shader(Inputs::ShaderCodes::vShader,
                                   Inputs::ShaderCodes::fShader);

  Chimia::Rendering::IndexedBuffer buffer(
    Inputs::BufferData::vertex,
    Inputs::BufferData::index,
    { Chimia::Rendering::ShaderAttribute::Float(0 /*position*/, 3 /*nFloats*/),
      Chimia::Rendering::ShaderAttribute::Float(1 /*UVs*/, 2 /*nFLoats*/) });

  int width, height, nChannels;

  const std::string assetsDir =
    SamplesUtils::GetCurrentAppDir(argv) + "/assets/";
  const std::string solarFlareAsset = assetsDir + "solar-flare.jpg";
  unsigned char* texData =
    stbi_load(solarFlareAsset.c_str(), &width, &height, &nChannels, 0);

  Chimia::Rendering::Texture2D texture(texData, width, height);

  stbi_image_free(texData);

  const Chimia::Rendering::TextureUnit texUnit =
    Chimia::Rendering::TextureUnit::UNIT_1;

  Chimia::Rendering::FrameBuffer frameBuffer(Inputs::SCR_WIDTH,
                                             Inputs::SCR_HEIGHT);

  Chimia::Rendering::Shader secondPassShader(Inputs::ShaderCodes::vShaderPost,
                                             Inputs::ShaderCodes::fShaderPost);

  Chimia::Rendering::IndexedBuffer quadBuffer(
    Inputs::BufferData::quad,
    Inputs::BufferData::quadIndex,
    { Chimia::Rendering::ShaderAttribute::Float(0 /*pos*/, 3 /*nFloats*/),
      Chimia::Rendering::ShaderAttribute::Float(1 /*uv*/, 2 /*nFloats*/) });

  const Chimia::Rendering::TextureUnit texUnitPost =
    Chimia::Rendering::TextureUnit::UNIT_2;

  while (!win.ShouldClose()) {
    frameBuffer.Use();

    texture.Use(texUnit);

    shader.Use();
    shader.SetUniform("tex", texUnit);
    buffer.Render();

    Chimia::Rendering::FrameBuffer::UseDefaultFrameBuffer();

    frameBuffer.UseTexture(texUnitPost);

    secondPassShader.Use();
    secondPassShader.SetUniform("tex", texUnitPost);
    quadBuffer.Render();

    win.Swap();
    win.PollEvents();
  }

  return 0;
}