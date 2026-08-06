#include "Core/Types.h"
#include "Media/Image.h"
#include "Rendering/DataLayout.h"
#include "Rendering/IndexData.h"
#include "Rendering/RenderAction.h"
#include "Rendering/Rendering.h"

#include "Rendering/Shader.h"
#include "Rendering/Texture2D.h"
#include "Rendering/TextureUnit.h"

#include "Rendering/VertexData.h"
#include "Utils/ExtrasUtils.h"
#include "Utils/Window.h"

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

// clang-format off
const std::vector<float> vertex{
 // x     y     z     u     v
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
    1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 
    0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
};
// clang-format on
const unsigned nVertices = 3;

const std::vector<unsigned> index{ 0, 1, 2 };

}

}

int
main(int argc, char** argv)
{
  Window win(1280, 1080, "Example #2");

  Chimia::Rendering::Initialize();

  auto shader = Chimia::Rendering::Shader::Create(
    Inputs::ShaderCodes::vShader,
    Inputs::ShaderCodes::fShader,
    { { "pos", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
      { "uv", Chimia::Rendering::eDataType::VECTOR_2_FLOAT } });

  auto vertexData = Chimia::Rendering::VertexData::New();
  vertexData->Create(
    Inputs::BufferData::vertex,
    { { "position", Chimia::Rendering::eDataType::VECTOR_3_FLOAT },
      { "texCoord", Chimia::Rendering::eDataType::VECTOR_2_FLOAT } });

  auto indexData = Chimia::Rendering::IndexData::New();
  indexData->Create(Inputs::BufferData::index);

  auto target = Chimia::Rendering::Target::Create(shader);

  Chimia::Rendering::RenderAction action;
  action.Create(
    target,
    indexData,
    { { vertexData, "position", "pos" }, { vertexData, "texCoord", "uv" } });

  const std::string assetsDir =
    ExtrasUtils::GetCurrentAppDir(argv) + "/assets/";

  const std::string blueLightAsset = assetsDir + "blue-light-style.jpg";
  Chimia::Media::Image texData(blueLightAsset.c_str());

  Chimia::Rendering::Texture2D texture;
  texture.Create(texData.RawData(), texData.Width(), texData.Height());

  const std::string solarFlareAsset = assetsDir + "solar-flare.jpg";
  Chimia::Media::Image texData2(solarFlareAsset.c_str());

  Chimia::Rendering::Texture2D texture2;
  texture2.Create(texData2.RawData(), texData2.Width(), texData2.Height());

  const Chimia::Rendering::TextureUnit texUnit =
    Chimia::Rendering::TextureUnit::UNIT_1;
  const Chimia::Rendering::TextureUnit tex2Unit =
    Chimia::Rendering::TextureUnit::UNIT_2;

  while (!win.ShouldClose()) {
    shader->SetTexture("tex", texture, texUnit);
    shader->SetTexture("tex2", texture2, tex2Unit);
    action.Render();

    win.Swap();
    win.PollEvents();
  }

  return 0;
}