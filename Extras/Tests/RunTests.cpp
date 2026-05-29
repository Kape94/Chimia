#include "Utils/SamplesUtils.h"

#include <iostream>

#include <cstdlib>
#include <map>
#include <string>
#include <vector>

int
main(int argc, char** argv)
{
  const std::string runnerPath = SamplesUtils::GetCurrentAppDir(argv);

  std::map<std::string, std::string> testCases{
    { "Rendering: #1 basic", "Rendering/Test_Rendering_1_basic" },
  };

  const size_t nTests = testCases.size();
  std::vector<std::string> failedTests;

  for (const auto& test : testCases) {
    const std::string testName = test.first;
    const std::string testApp = test.second;

    std::cout << "Starting test " << testName << "...\n";

    const std::string testCommand = runnerPath + testApp;
    const int returnCode = std::system(testCommand.c_str());

    std::string testStatus = returnCode != 0 ? "[FAIL]" : "[PASS]";
    std::cout << testStatus << "\t" << testName << "\n\n";

    if (returnCode != 0) {
      failedTests.push_back(testName);
    }
  }

  const size_t nFailedTests = failedTests.size();

  const float failedRate = (float)nFailedTests / nTests;

  std::cout << "------------ Test summary ------------\n";
  std::cout << "Tests ran: " << nTests << "\n";
  std::cout << "Tests failed: " << nFailedTests << "\n";

  if (nFailedTests > 0) {
    std::cout << "\nThe following tests failed...\n\n";
    for (const std::string& failed : failedTests) {
      std::cout << failed << "\n";
    }
  }

  std::cout << "\n\n------------ Tests finished ------------\n\n";

  return 0;
}