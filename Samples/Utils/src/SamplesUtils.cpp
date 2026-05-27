#include "SamplesUtils.h"

#include "OpenGLHelpers.h"
#include <cassert>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

  std::vector<GLubyte> pixels(width * height * nComp);

  glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

  stbi_flip_vertically_on_write(true);
  stbi_write_bmp(imagePath.c_str(), width, height, nComp, pixels.data());
}

// ----------------------------------------------------------------------------

SamplesUtils::Image
SamplesUtils::ReadImage(const std::string& imagePath)
{
  int width = 0, height = 0, nChannels = 0;

  unsigned char* data =
    stbi_load(imagePath.c_str(), &width, &height, &nChannels, 0);

  return Image{ data, width, height, nChannels };
}

// ----------------------------------------------------------------------------

glm::vec3
SamplesUtils::GetRGBPixel(const int i, const int j, const Image& image)
{
  assert(image.nChannels == 3 && "GetRGBAPixel: image doesn't have 3 channels");
  const unsigned char* data = image.data;

  const int offset = image.nChannels * (image.width * j + i);
  const unsigned char* pixelData = data + offset;

  auto normalized = [](const unsigned char v) -> float {
    return static_cast<float>(v) / 256.0f;
  };

  const unsigned char r = pixelData[0];
  const unsigned char g = pixelData[1];
  const unsigned char b = pixelData[2];

  return { normalized(r), normalized(g), normalized(b) };
}

// ----------------------------------------------------------------------------

glm::vec4
SamplesUtils::GetRGBAPixel(const int i, const int j, const Image& image)
{
  assert(image.nChannels == 4 && "GetRGBAPixel: image doesn't have 4 channels");
  const unsigned char* data = image.data;

  const int offset = image.nChannels * (image.width * j + i);
  const unsigned char* pixelData = data + offset;

  auto normalized = [](const unsigned char v) -> float {
    return static_cast<float>(v) / 256.0f;
  };

  const unsigned char r = pixelData[0];
  const unsigned char g = pixelData[1];
  const unsigned char b = pixelData[2];
  const unsigned char a = pixelData[3];

  return { normalized(r), normalized(g), normalized(b), normalized(a) };
}

// ----------------------------------------------------------------------------

void
SamplesUtils::SetRGBPixel(const glm::vec3& color,
                          const int i,
                          const int j,
                          Image& image)
{
  assert(image.nChannels == 3 && "SetRGBPixel: image doesn't have 3 channels");
  unsigned char* data = image.data;

  const int offset = image.nChannels * (image.width * j + i);
  unsigned char* pixelData = data + offset;

  auto toUnsigned = [](const float v) -> unsigned char {
    return static_cast<unsigned char>(v * 256.0f);
  };

  pixelData[0] = toUnsigned(color.r);
  pixelData[1] = toUnsigned(color.g);
  pixelData[2] = toUnsigned(color.b);
}

// ----------------------------------------------------------------------------

void
SamplesUtils::SetRGBAPixel(const glm::vec4& color,
                           const int i,
                           const int j,
                           Image& image)
{
  assert(image.nChannels == 4 && "SetRGBAPixel: image doesn't have 4 channels");
  unsigned char* data = image.data;

  const int offset = image.nChannels * (image.width * j + i);
  unsigned char* pixelData = data + offset;

  auto toUnsigned = [](const float v) -> unsigned char {
    return static_cast<unsigned char>(v * 256.0f);
  };

  pixelData[0] = toUnsigned(color.r);
  pixelData[1] = toUnsigned(color.g);
  pixelData[2] = toUnsigned(color.b);
  pixelData[3] = toUnsigned(color.a);
}

// ----------------------------------------------------------------------------

float
SamplesUtils::ImageDifferece(const Image& i1, const Image& i2)
{
  if (i1.width != i2.width || i1.height != i2.height ||
      i1.nChannels != i2.nChannels) {
    return 100.0f;
  }

  Image diffImage;
  diffImage.width = i1.width;
  diffImage.height = i1.height;
  diffImage.nChannels = i1.nChannels;
  diffImage.data =
    new unsigned char[diffImage.width * diffImage.height * diffImage.nChannels];

  float diff = 0.0f;
  float totalPixelArea = (float)i1.width * i1.height;
  float pixelContribution = 1.0f / totalPixelArea;
  if (i1.nChannels == 3) {
    for (int i = 0; i < i1.width; ++i) {
      for (int j = 0; j < i1.height; ++j) {
        const glm::vec3 c1 = GetRGBPixel(i, j, i1);
        const glm::vec3 c2 = GetRGBPixel(i, j, i2);

        diff +=
          pixelContribution * (std::abs(c1.r - c2.r) + std::abs(c1.g - c2.g) +
                               std::abs(c1.b - c2.b));

        glm::vec3 diffPixel{ std::abs(c1.r - c2.r),
                             std::abs(c1.g - c2.g),
                             std::abs(c1.b - c2.b) };
        SetRGBPixel(diffPixel, i, j, diffImage);
      }
    }
  } else if (i1.nChannels == 4) {
    for (int i = 0; i < i1.width; ++i) {
      for (int j = 0; j < i1.height; ++j) {
        const glm::vec4 c1 = GetRGBAPixel(i, j, i1);
        const glm::vec4 c2 = GetRGBAPixel(i, j, i2);

        diff +=
          pixelContribution * (std::abs(c1.r - c2.r) + std::abs(c1.g - c2.g) +
                               std::abs(c1.b - c2.b) + std::abs(c1.a - c2.a));
        glm::vec4 diffPixel{ std::abs(c1.r - c2.r),
                             std::abs(c1.g - c2.g),
                             std::abs(c1.b - c2.b),
                             std::abs(c1.a - c2.a) };
        SetRGBAPixel(diffPixel, i, j, diffImage);
      }
    }
  } else {
    diff = 100.0f;
  }

  stbi_flip_vertically_on_write(false);
  stbi_write_bmp("diff.bmp",
                 diffImage.width,
                 diffImage.height,
                 diffImage.nChannels,
                 diffImage.data);

  FreeImageData(diffImage);

  return diff;
}

// ----------------------------------------------------------------------------

void
SamplesUtils::FreeImageData(SamplesUtils::Image& image)
{
  stbi_image_free(image.data);
}

// ----------------------------------------------------------------------------