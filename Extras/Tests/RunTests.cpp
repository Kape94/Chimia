#include "Utils/ExtrasUtils.h"

#include <chrono>
#include <iostream>

#include <cstdlib>
#include <string>
#include <vector>

namespace Timing {
std::chrono::time_point<std::chrono::high_resolution_clock> g_InitTime;

std::chrono::time_point<std::chrono::high_resolution_clock>
Now()
{
  return std::chrono::high_resolution_clock::now();
}

void
Tic()
{
  g_InitTime = Now();
}

float
Toc()
{
  const auto finalTime = Now();
  const auto diff = finalTime - g_InitTime;

  const long long millisecs =
    std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();
  return static_cast<float>(millisecs) / 1000.0f;
}

}

int
main(int argc, char** argv)
{
  const std::string runnerPath = ExtrasUtils::GetCurrentAppDir(argv);

  std::vector<std::pair<std::string, std::string>> testCases{
    { "Rendering: #1 basic", "Rendering/Test_Rendering_1_basic" },
    { "Rendering: #2 texture", "Rendering/Test_Rendering_2_texture" },
    { "Rendering: #3 framebuffer", "Rendering/Test_Rendering_3_framebuffer" },
    { "Rendering: #4 shader uniform",
      "Rendering/Test_Rendering_4_shaderUniform" },
    { "Rendering: #5 instanced buffer",
      "Rendering/Test_Rendering_5_instancedBuffer" },
    { "Rendering: #6 buffer", "Rendering/Test_Rendering_6_buffer" },
    { "Rendering: #7 reusableBuffer",
      "Rendering/Test_Rendering_7_reusableBuffer" },
    { "Rendering: #8 color blending",
      "Rendering/Test_Rendering_8_colorBlending" },

    { "Graphics: #1 basic", "Graphics/Test_Graphics_1_basic" },
    { "Graphics: #2 immediate mode triangles",
      "Graphics/Test_Graphics_2_trianglesImmediate" },
    { "Graphics: #3 batching", "Graphics/Test_Graphics_3_batching" },
    { "Graphics: #4 retained mode triangles",
      "Graphics/Test_Graphics_4_trianglesRetained" },
    { "Graphics: #5 immediate mode models",
      "Graphics/Test_Graphics_5_modelsImmediate" },
    { "Graphics: #6 retained mode models",
      "Graphics/Test_Graphics_6_modelsRetained" },
  };

  const std::string failText = "[\033[1;31m FAIL \033[0m ]";
  const std::string passText = "[\033[1;32m PASS \033[0m ]";

  const size_t nTests = testCases.size();
  std::vector<std::string> failedTests;
  float totalTestingTime = 0.0f;

  for (const auto& test : testCases) {
    const std::string testName = test.first;
    const std::string testApp = test.second;

    std::cout << "Starting test " << testName << "...\n";

    const std::string testCommand = runnerPath + testApp;

    Timing::Tic();
    const int returnCode = std::system(testCommand.c_str());
    const float testExecutionTime = Timing::Toc();

    std::string testStatus = returnCode != 0 ? failText : passText;
    std::cout << testStatus << "\t" << testName << "\n";
    std::cout << "Executed in " << testExecutionTime << " seconds\n\n";

    if (returnCode != 0) {
      failedTests.push_back(testName);
    }
    totalTestingTime += testExecutionTime;
  }

  const size_t nFailedTests = failedTests.size();

  std::cout << "------------ Test summary ------------\n";
  std::cout << "Tests ran: " << nTests << "\n";
  std::cout << "Tests failed: " << nFailedTests << "\n";
  std::cout << "Total testing time (seconds): " << totalTestingTime << "\n";

  if (nFailedTests > 0) {
    std::cout << "\nThe following tests failed...\n\n";
    for (const std::string& failed : failedTests) {
      std::cout << failed << "\n";
    }
  }

  std::cout << "\n\n------------ Tests finished ------------\n\n";

  return 0;
}