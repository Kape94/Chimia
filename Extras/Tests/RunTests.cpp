#include "DummyTests.h"

#include <iostream>

#include <functional>
#include <map>
#include <string>
#include <vector>

using TestCase = std::function<int()>;

int
main()
{
  std::map<std::string, TestCase> testCases{
    { "Rendering: test #1", &DummyTests::test1 },
    { "Rendering: test #2", &DummyTests::test2 }
  };

  const size_t nTests = testCases.size();

  std::vector<std::string> failedTests;

  for (const auto& test : testCases) {
    const std::string testName = test.first;
    const TestCase& testFunction = test.second;

    std::cout << "Starting test " << testName << "...\n";

    const int returnCode = testFunction();

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