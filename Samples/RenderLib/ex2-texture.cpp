#include "RenderLib/RenderLib.h"

#include "RenderLib/IndexedBuffer.h"
#include "RenderLib/Shader.h"
#include "RenderLib/Texture2D.h"
#include "RenderLib/TextureUnit.h"

#include "Utils/SamplesUtils.h"
#include "Utils/Window.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Utils/stb_image.h"

namespace Inputs {

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
      uniform sampler2D tex2;

      void main() {
        fragColor = texture(tex2, textureUV) + texture(tex, textureUV);
      }
    )";

}

namespace BufferData {

const std::vector<float> vertex{
  // x ,    y,    z,    u,    v,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
  1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
};

const std::vector<unsigned> index{ 0, 1, 2 };

}

}

int
main(int argc, char** argv)
{
  Window win(1280, 1080, "Example #2");

  RenderLib::Initialize();

  RenderLib::Shader shader;
  shader.Create(Inputs::ShaderCodes::vShader, Inputs::ShaderCodes::fShader);

  RenderLib::IndexedBuffer buffer;
  buffer.Create(
    Inputs::BufferData::vertex,
    Inputs::BufferData::index,
    { RenderLib::ShaderAttribute::Float(0 /*position*/, 3 /*nFloats*/),
      RenderLib::ShaderAttribute::Float(1 /*UVs*/, 2 /*nFLoats*/) });

  const std::string assetsDir =
    SamplesUtils::GetCurrentAppDir(argv) + "/assets/";

  int width, height, nChannels;

  const std::string blueLightAsset = assetsDir + "blue-light-style.jpg";
  unsigned char* texData =
    stbi_load(blueLightAsset.c_str(), &width, &height, &nChannels, 0);

  RenderLib::Texture2D texture;
  texture.Create(texData, width, height);

  stbi_image_free(texData);

  const std::string solarFlareAsset = assetsDir + "solar-flare.jpg";
  texData = stbi_load(solarFlareAsset.c_str(), &width, &height, &nChannels, 0);

  RenderLib::Texture2D texture2;
  texture2.Create(texData, width, height);

  stbi_image_free(texData);

  const RenderLib::TextureUnit texUnit = RenderLib::TextureUnit::UNIT_1;
  const RenderLib::TextureUnit tex2Unit = RenderLib::TextureUnit::UNIT_2;

  while (!win.ShouldClose()) {
    texture.Use(texUnit);
    texture2.Use(tex2Unit);

    shader.Use();
    shader.SetUniform("tex", texUnit);
    shader.SetUniform("tex2", tex2Unit);
    buffer.Render();

    win.Swap();
    win.PollEvents();
  }

  buffer.Clear();
  texture.Clear();
  texture2.Clear();
  shader.Clear();

  return 0;
}