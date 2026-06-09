#include "ExtrasUtils.h"

#include "Media/Image.h"
#include "Media/ImageFormat.h"
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

namespace SamplesUtilsInternal {
void
SaveScreenshot(const Window& window,
               const std::string& imagePath,
               const int nChannels)
{
  const auto [width, height] = window.GetFramebufferSize();

  std::vector<unsigned char> pixels(width * height * nChannels);

  glReadBuffer(GL_FRONT);

  const int format = nChannels == 4 ? GL_RGBA : GL_RGB;
  glReadPixels(0, 0, width, height, format, GL_UNSIGNED_BYTE, pixels.data());

  Chimia::Media::Image image(width, height, nChannels, pixels.data());
  image.SaveAsPng(imagePath, true /*flipVertically*/);
}
}

// ----------------------------------------------------------------------------

std::string
ExtrasUtils::GetCurrentAppDir(char** argv)
{
  const std::filesystem::path p = argv[0];
  const std::filesystem::path current = std::filesystem::current_path();

  std::filesystem::path finalPath = current / p;
  finalPath.remove_filename();

  return finalPath.string();
}

// ----------------------------------------------------------------------------

void
ExtrasUtils::InitRandom()
{
  srand(time(NULL));
}

// ----------------------------------------------------------------------------

float
ExtrasUtils::NormalizedRand()
{
  return (float)rand() / RAND_MAX;
}

// ----------------------------------------------------------------------------

int
ExtrasUtils::Rand()
{
  return rand();
}

// ----------------------------------------------------------------------------

void
ExtrasUtils::SyncForTargetFPS(unsigned fps)
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
ExtrasUtils::BeginFrameStats()
{
  g_frameInit = CurrentTimePoint();
}

// ----------------------------------------------------------------------------

void
ExtrasUtils::LogFrameStats()
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
ExtrasUtils::DoAfter(const std::function<void(void)>& action,
                     const unsigned milliseconds)
{
  ThreadPoolLite::PushTask([action, milliseconds]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));

    action();
  });
}

// ----------------------------------------------------------------------------

void
ExtrasUtils::DoAfterSync(const std::function<void(void)>& action,
                         const unsigned milliseconds)
{
  ThreadPoolLite::PushTask([action, milliseconds]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));

    FunctionQueue::MoveToQueue(action);
  });
}

// ----------------------------------------------------------------------------

void
ExtrasUtils::PollDeferredActions()
{
  FunctionQueue::PollQueue();
}

// ----------------------------------------------------------------------------

void
ExtrasUtils::PollSingleDeferredAction()
{
  FunctionQueue::PollSingleFromQueue();
}

// ----------------------------------------------------------------------------

void
ExtrasUtils::SaveRGBScreenshot(const Window& window,
                               const std::string& imagePath)
{
  SamplesUtilsInternal::SaveScreenshot(window, imagePath, 3 /*nChannels*/);
}

// ----------------------------------------------------------------------------

void
ExtrasUtils::SaveRGBAScreenshot(const Window& window,
                                const std::string& imagePath)
{
  SamplesUtilsInternal::SaveScreenshot(window, imagePath, 4 /*nChannels*/);
}

// ----------------------------------------------------------------------------

Chimia::Media::ImageFormat
ExtrasUtils::GetFileNameFormat(const std::string& fileName)
{
  const size_t dotIndex = fileName.rfind('.');

  const std::string fileExtension(fileName.begin() + dotIndex + 1,
                                  fileName.end());

  return Chimia::Media::ImageFormat::FromString(fileExtension);
}

// ----------------------------------------------------------------------------