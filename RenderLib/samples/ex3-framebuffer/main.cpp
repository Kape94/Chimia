#include "RenderLib/RenderLib.h"

#include "RenderLib/FrameBuffer.h"
#include "RenderLib/IndexedBuffer.h"
#include "RenderLib/Shader.h"
#include "RenderLib/Texture2D.h"
#include "RenderLib/TextureUnit.h"

#include "Utils/Window.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

const std::vector<float> vertex{
  // x ,    y,    z,    u,    v,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
  1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
};

const std::vector<unsigned> index{ 0, 1, 2 };

const std::vector<float> quad{ // x,   y,    z,    u,    v
                               -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, -1.0f,
                               0.0f,  1.0f,  0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
                               1.0f,  -1.0f, 1.0f, 0.0f, 0.0f, 1.0f
};

const std::vector<unsigned> quadIndex{ 0, 1, 2, 2, 3, 0 };

}

}

int
main()
{
  Window win(Inputs::SCR_WIDTH, Inputs::SCR_HEIGHT, "Example #3");

  RenderLib::Initialize();

  RenderLib::SetViewport(0, 0, Inputs::SCR_WIDTH, Inputs::SCR_HEIGHT);

  RenderLib::Shader shader(Inputs::ShaderCodes::vShader,
                           Inputs::ShaderCodes::fShader);

  RenderLib::IndexedBuffer buffer(
    Inputs::BufferData::vertex,
    Inputs::BufferData::index,
    { RenderLib::ShaderAttribute::Float(0 /*position*/, 3 /*nFloats*/),
      RenderLib::ShaderAttribute::Float(1 /*UVs*/, 2 /*nFLoats*/) });

  int width, height, nChannels;
  unsigned char* texData =
    stbi_load("solar-flare.jpg", &width, &height, &nChannels, 0);

  RenderLib::Texture2D texture(texData, width, height);

  stbi_image_free(texData);

  const RenderLib::TextureUnit texUnit = RenderLib::TextureUnit::UNIT_1;

  RenderLib::FrameBuffer frameBuffer(Inputs::SCR_WIDTH, Inputs::SCR_HEIGHT);

  RenderLib::Shader secondPassShader(Inputs::ShaderCodes::vShaderPost,
                                     Inputs::ShaderCodes::fShaderPost);

  RenderLib::IndexedBuffer quadBuffer(
    Inputs::BufferData::quad,
    Inputs::BufferData::quadIndex,
    { RenderLib::ShaderAttribute::Float(0 /*pos*/, 3 /*nFloats*/),
      RenderLib::ShaderAttribute::Float(1 /*uv*/, 2 /*nFloats*/) });

  const RenderLib::TextureUnit texUnitPost = RenderLib::TextureUnit::UNIT_2;

  while (!win.ShouldClose()) {
    frameBuffer.Use();

    texture.Use(texUnit);

    shader.Use();
    shader.SetUniform("tex", texUnit);
    buffer.Render();

    RenderLib::FrameBuffer::UseDefaultFrameBuffer();

    frameBuffer.UseTexture(texUnitPost);

    secondPassShader.Use();
    secondPassShader.SetUniform("tex", texUnitPost);
    quadBuffer.Render();

    win.Swap();
    win.PollEvents();
  }

  return 0;
}