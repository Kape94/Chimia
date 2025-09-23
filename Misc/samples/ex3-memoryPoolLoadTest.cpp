#include <iostream>

#include <chrono>
#include <vector>

#include "Misc/MemoryPool/MemoryPool.h"

struct Data
{
  std::vector<int> v;
  std::string s;
};

void
InitData(Data& data)
{
  data.s = "SomeLenghtyLenghtyTextkkkkkkkkkkkkkk";
  data.v.assign({ 4, 5, 10, 15 });
}

constexpr unsigned N_TEST_RUNS = 10;
constexpr unsigned N_OBJECTS_TO_ALLOC = 100000;

int
main()
{
  {
    std::vector<Data*> datasPlainAlloc;
    datasPlainAlloc.reserve(N_OBJECTS_TO_ALLOC);

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N_TEST_RUNS; ++i) {
      for (int j = 0; j < N_OBJECTS_TO_ALLOC; ++j) {
        auto d = new Data;
        InitData(*d);
        datasPlainAlloc.push_back(d);
      }
      for (int j = 0; j < N_OBJECTS_TO_ALLOC; ++j) {
        delete datasPlainAlloc[j];
      }
      datasPlainAlloc.clear();
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = end - start;
    const long ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    std::cout << "Test with plain alloc: " << ms << " ms" << std::endl;
  }

  auto startPool = std::chrono::high_resolution_clock::now();
  {

    Misc::MemoryPool<Data> pool(N_OBJECTS_TO_ALLOC);
    std::vector<Data*> datasPoolAlloc;
    datasPoolAlloc.reserve(N_OBJECTS_TO_ALLOC);

    for (int i = 0; i < N_TEST_RUNS; ++i) {
      for (int j = 0; j < N_OBJECTS_TO_ALLOC; ++j) {
        auto d = pool.New();
        InitData(*d);
        datasPoolAlloc.push_back(d);
      }
      for (int j = 0; j < N_OBJECTS_TO_ALLOC; ++j) {
        pool.Release(datasPoolAlloc[j]);
      }
      datasPoolAlloc.clear();
    }
  }
  auto endPool = std::chrono::high_resolution_clock::now();
  auto durationPool = endPool - startPool;
  const long msPool =
    std::chrono::duration_cast<std::chrono::milliseconds>(durationPool).count();
  std::cout << "Test with pool alloc: " << msPool << " ms" << std::endl;

  return 0;
}