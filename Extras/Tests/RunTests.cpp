#include "Utils/ExtrasUtils.h"

#include <iostream>

#include <cstdlib>
#include <set>
#include <string>
#include <vector>

// ----------------------------------------------------------------------------

namespace Grouping {
enum class Group
{
  ALL,
  RENDERING,
  GRAPHICS
};

Group
FromString(const std::string& groupName)
{
  if (groupName == "rendering")
    return Group::RENDERING;
  if (groupName == "graphics")
    return Group::GRAPHICS;
  else
    return Group::ALL;
}

using Groups = std::set<Group>;

bool
ContainsGroup(const Group group, const Groups& groups)
{
  return groups.count(group) > 0;
}

const Groups renderingGroup{ Group::RENDERING, Group::ALL };
const Groups graphicsGroup{ Group::GRAPHICS, Group::ALL };

}

// ----------------------------------------------------------------------------

struct Test
{
  std::string name;
  std::string exePath;
  Grouping::Groups groups;
};

Test
RenderingTest(const std::string& testName, const std::string& testExe)
{
  return { testName, testExe, Grouping::renderingGroup };
}

Test
GraphicsTest(const std::string& testName, const std::string& testExe)
{
  return { testName, testExe, Grouping::graphicsGroup };
}

// ----------------------------------------------------------------------------

int
main(int argc, char** argv)
{
  Grouping::Group selectedGroup = Grouping::Group::ALL;
  if (argc > 1) {
    const std::string groupName(argv[1]);
    selectedGroup = Grouping::FromString(groupName);
  }

  const std::string runnerPath = ExtrasUtils::GetCurrentAppDir(argv);

  std::vector<Test> testCases{
    RenderingTest("Rendering: #1 basic", "Rendering/Test_Rendering_1_basic"),
    RenderingTest("Rendering: #2 texture",
                  "Rendering/Test_Rendering_2_texture"),
    RenderingTest("Rendering: #3 framebuffer",
                  "Rendering/Test_Rendering_3_framebuffer"),
    RenderingTest("Rendering: #4 shader uniform",
                  "Rendering/Test_Rendering_4_shaderUniform"),
    RenderingTest("Rendering: #5 instanced buffer",
                  "Rendering/Test_Rendering_5_instancedBuffer"),
    RenderingTest("Rendering: #6 buffer", "Rendering/Test_Rendering_6_buffer"),
    RenderingTest("Rendering: #7 reusableBuffer",
                  "Rendering/Test_Rendering_7_reusableBuffer"),
    RenderingTest("Rendering: #8 color blending",
                  "Rendering/Test_Rendering_8_colorBlending"),

    GraphicsTest("Graphics: #1 basic", "Graphics/Test_Graphics_1_basic"),
    GraphicsTest("Graphics: #2 immediate mode triangles",
                 "Graphics/Test_Graphics_2_trianglesImmediate"),
    GraphicsTest("Graphics: #3 batching", "Graphics/Test_Graphics_3_batching"),
    GraphicsTest("Graphics: #4 retained mode triangles",
                 "Graphics/Test_Graphics_4_trianglesRetained"),
    GraphicsTest("Graphics: #5 immediate mode models",
                 "Graphics/Test_Graphics_5_modelsImmediate"),
    GraphicsTest("Graphics: #6 retained mode models",
                 "Graphics/Test_Graphics_6_modelsRetained"),
    GraphicsTest("Graphics: #7 performance simple",
                 "Graphics/Test_Graphics_7_performanceSimple"),
  };

  const std::string failText = "[\033[1;31m FAIL \033[0m ]";
  const std::string passText = "[\033[1;32m PASS \033[0m ]";

  const size_t nTests = testCases.size();
  std::vector<std::string> failedTests;
  float totalTestingTime = 0.0f;
  size_t testsRun = 0;

  for (const auto& test : testCases) {
    const std::string testName = test.name;
    const std::string testApp = test.exePath;

    if (!Grouping::ContainsGroup(selectedGroup, test.groups)) {
      continue;
    }

    std::cout << "Starting test " << testName << "...\n";

    const std::string testCommand = runnerPath + testApp;

    ExtrasUtils::Tic();
    const int returnCode = std::system(testCommand.c_str());
    const float testExecutionTime = ExtrasUtils::Toc();

    std::string testStatus = returnCode != 0 ? failText : passText;
    std::cout << testStatus << "\t" << testName << "\n";
    std::cout << "Executed in " << testExecutionTime << " seconds\n\n";

    if (returnCode != 0) {
      failedTests.push_back(testName);
    }
    totalTestingTime += testExecutionTime;
    ++testsRun;
  }

  const size_t nFailedTests = failedTests.size();

  std::cout << "------------ Test summary ------------\n";
  std::cout << "Tests ran: " << testsRun << "\n";
  std::cout << "Total tests: " << nTests << "\n";
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

// ----------------------------------------------------------------------------