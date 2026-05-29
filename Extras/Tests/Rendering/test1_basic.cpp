#include "Media/Image.h"
#include "Rendering/Rendering.h"

#include "Rendering/IndexedBuffer.h"
#include "Rendering/Shader.h"

#include "Utils/ImageUtils.h"
#include "Utils/SamplesUtils.h"
#include "Utils/Window.h"

#include <iostream>

namespace Inputs {

namespace ShaderCodes {

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

}

namespace BufferData {

// clang-format off
const float vertex[] = { 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
                         1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
                         0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };
// clang-format on

const unsigned vertexDataSize = 18 * sizeof(float);

const unsigned indexData[] = { 0, 1, 2 };

const unsigned indexDataNItems = 3;
}
}

int
main(int argc, char** argv)
{
  Window win(1280, 1080, "Rendering test #1");

  const std::string testPath = SamplesUtils::GetCurrentAppDir(argv);
  const std::string goldenArtifactsDir = testPath + "/goldenArtifacts/";

  Chimia::Rendering::Initialize();

  Chimia::Rendering::Shader shader;
  shader.Create(Inputs::ShaderCodes::vShader, Inputs::ShaderCodes::fShader);

  Chimia::Rendering::IndexedBuffer buffer;
  buffer.Create(
    { Inputs::BufferData::vertex, Inputs::BufferData::vertexDataSize },
    { Inputs::BufferData::indexData, Inputs::BufferData::indexDataNItems },
    { Chimia::Rendering::ShaderAttribute::Float(0 /*position*/, 3 /*nFloats*/),
      Chimia::Rendering::ShaderAttribute::Float(1 /*color*/, 3 /*nFLoats*/) });

  shader.Use();
  buffer.Render();

  win.Swap();

  SamplesUtils::SaveScreenshot(win, testPath + "/output.bmp");
  Chimia::Media::Image output(testPath + "/output.bmp");

  Chimia::Media::Image goldenImage(goldenArtifactsDir + "test1_basic.bmp");

  const ImageUtils::ImageComparisonResult result =
    ImageUtils::Compare(output, goldenImage);

  const bool comparedWithoutErrors = result.errorMessage.empty();
  if (comparedWithoutErrors) {
    if (result.diff > 0.00001f) {
      std::cout << "DIFF: " << result.diff << "\n\n";
      result.diffImage.SaveAsBmp("diff.bmp", false /*flipVertically*/);

      return 1;
    }
  } else {
    std::cout << "Couldn't compare the images:\n"
              << result.errorMessage << "\n";
    return 1;
  }

  return 0;
}
