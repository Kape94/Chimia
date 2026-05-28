#include "SamplesUtils.h"

#include "Media/Image.h"
#include "OpenGLHelpers.h"
#include <cassert>

#include <chrono>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

// ----------------------------------------------------------------------------

namespace {
std::chrono::time_point<std::chrono::high_resolution_clock>
CurrentTimePoint()
{
  return std::chrono::high_resolution_clock::now();
}

std::chrono::time_point<std::chrono::high_resolution_clock> g_lastCheck =
  CurrentTimePoint();

std::chrono::time_point<std::chrono::high_resolution_clock> g_frameInit =
  CurrentTimePoint();
long g_bestFrameTime = 10000000;
long g_worstFrameTime = 0;
}

// ----------------------------------------------------------------------------

namespace ThreadPoolLite {
class AutoJoiningThread
{
public:
  template<class Function, class... Args>
  AutoJoiningThread(Function f, Args... args)
    : m_thread(f, std::forward<Args>(args)...)
  {
  }

  ~AutoJoiningThread()
  {
    if (m_thread.joinable())
      m_thread.join();
  }

private:
  std::thread m_thread;
};

std::vector<std::unique_ptr<AutoJoiningThread>> threads;

void
PushTask(std::function<void(void)> action)
{
  threads.push_back(std::make_unique<AutoJoiningThread>(action));
}

}

// ----------------------------------------------------------------------------

namespace FunctionQueue {
std::queue<std::function<void(void)>> m_functionQueue;
std::mutex m_queueMutex;

void
MoveToQueue(const std::function<void(void)>& function)
{
  std::lock_guard lock(m_queueMutex);
  m_functionQueue.push(function);
}

void
PollQueue()
{
  std::lock_guard lock(m_queueMutex);
  while (m_functionQueue.size() > 0) {
    auto function = m_functionQueue.front();
    function();
    m_functionQueue.pop();
  }
}

void
PollSingleFromQueue()
{
  std::lock_guard lock(m_queueMutex);
  if (m_functionQueue.size() > 0) {
    auto function = m_functionQueue.front();
    function();
    m_functionQueue.pop();
  }
}

}

// ----------------------------------------------------------------------------

std::string
SamplesUtils::GetCurrentAppDir(char** argv)
{
  const std::filesystem::path p = argv[0];
  const std::filesystem::path current = std::filesystem::current_path();

  std::filesystem::path finalPath = current / p;
  finalPath.remove_filename();

  return finalPath.string();
}

// ----------------------------------------------------------------------------

void
SamplesUtils::InitRandom()
{
  srand(time(NULL));
}

// ----------------------------------------------------------------------------

float
SamplesUtils::NormalizedRand()
{
  return (float)rand() / RAND_MAX;
}

// ----------------------------------------------------------------------------

int
SamplesUtils::Rand()
{
  return rand();
}

// ----------------------------------------------------------------------------

void
SamplesUtils::SyncForTargetFPS(unsigned fps)
{
  const float frameTime = 1.0f / (float)fps;
  const long frameTimeInMilisecs = (long)(frameTime * 1000.0f);

  const auto current = CurrentTimePoint();
  const auto diff = current - g_lastCheck;

  const long durationInMilliseconds =
    std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();
  const long remainingFrameTime = frameTimeInMilisecs - durationInMilliseconds;

  std::this_thread::sleep_for(std::chrono::milliseconds(remainingFrameTime));
  g_lastCheck = CurrentTimePoint();
}

// ----------------------------------------------------------------------------

void
SamplesUtils::BeginFrameStats()
{
  g_frameInit = CurrentTimePoint();
}

// ----------------------------------------------------------------------------

void
SamplesUtils::LogFrameStats()
{
  auto now = CurrentTimePoint();
  auto diff = now - g_frameInit;

  bool shouldPrintStats = false;
  const long diffMilli =
    std::chrono::duration_cast<std::chrono::microseconds>(diff).count();
  if (diffMilli < g_bestFrameTime) {
    g_bestFrameTime = diffMilli;
    shouldPrintStats = true;
  }
  if (diffMilli > g_worstFrameTime) {
    g_worstFrameTime = diffMilli;
    shouldPrintStats = true;
  }

  if (shouldPrintStats) {
    std::cout << "Updated frame time stats...\n";
    std::cout << "Best frame time: " << g_bestFrameTime << " us\n";
    std::cout << "Worst frame time: " << g_worstFrameTime << " us\n";
  }
}

// ----------------------------------------------------------------------------

void
SamplesUtils::DoAfter(const std::function<void(void)>& action,
                      const unsigned milliseconds)
{
  ThreadPoolLite::PushTask([action, milliseconds]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));

    action();
  });
}

// ----------------------------------------------------------------------------

void
SamplesUtils::DoAfterSync(const std::function<void(void)>& action,
                          const unsigned milliseconds)
{
  ThreadPoolLite::PushTask([action, milliseconds]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));

    FunctionQueue::MoveToQueue(action);
  });
}

// ----------------------------------------------------------------------------

void
SamplesUtils::PollDeferredActions()
{
  FunctionQueue::PollQueue();
}

// ----------------------------------------------------------------------------

void
SamplesUtils::PollSingleDeferredAction()
{
  FunctionQueue::PollSingleFromQueue();
}

// ----------------------------------------------------------------------------

void
SamplesUtils::SaveScreenshot(const Window& window, const std::string& imagePath)
{
  const auto [width, height] = window.GetFramebufferSize();
  const int nComp = 3;

  std::vector<unsigned char> pixels(width * height * nComp);

  glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

  Chimia::Media::Image image(width, height, nComp, pixels.data());
  image.Save("", true /*flipVertically*/);
}

// ----------------------------------------------------------------------------

float
SamplesUtils::ImageDifferece(const Chimia::Media::Image& i1,
                             const Chimia::Media::Image& i2)
{
  if (i1.Width() != i2.Width() || i1.Height() != i2.Height() ||
      i1.NChannels() != i2.NChannels()) {
    return 100.0f;
  }

  Chimia::Media::Image diffImage(i1.Width(), i1.Height(), i1.NChannels());

  float diff = 0.0f;
  float totalPixelArea = (float)i1.Width() * i1.Height();
  float pixelContribution = 1.0f / totalPixelArea;
  if (i1.NChannels() == 3) {
    for (int i = 0; i < i1.Width(); ++i) {
      for (int j = 0; j < i1.Height(); ++j) {
        const glm::vec3 c1 = i1.GetRGBPixel(i, j);
        const glm::vec3 c2 = i2.GetRGBPixel(i, j);

        diff +=
          pixelContribution * (std::abs(c1.r - c2.r) + std::abs(c1.g - c2.g) +
                               std::abs(c1.b - c2.b));

        glm::vec3 diffPixel{ std::abs(c1.r - c2.r),
                             std::abs(c1.g - c2.g),
                             std::abs(c1.b - c2.b) };
        diffImage.SetRGBPixel(diffPixel, i, j);
      }
    }
  } else if (i1.NChannels() == 4) {
    for (int i = 0; i < i1.Width(); ++i) {
      for (int j = 0; j < i1.Height(); ++j) {
        const glm::vec4 c1 = i1.GetRGBAPixel(i, j);
        const glm::vec4 c2 = i2.GetRGBAPixel(i, j);

        diff +=
          pixelContribution * (std::abs(c1.r - c2.r) + std::abs(c1.g - c2.g) +
                               std::abs(c1.b - c2.b) + std::abs(c1.a - c2.a));
        glm::vec4 diffPixel{ std::abs(c1.r - c2.r),
                             std::abs(c1.g - c2.g),
                             std::abs(c1.b - c2.b),
                             std::abs(c1.a - c2.a) };
        diffImage.SetRGBAPixel(diffPixel, i, j);
      }
    }
  } else {
    diff = 100.0f;
  }

  diffImage.Save("diff.bmp", false /*flipVertically*/);

  return diff;
}

// ----------------------------------------------------------------------------