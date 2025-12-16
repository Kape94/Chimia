#include <iostream>

#include "Core/DiskOffloadableDataBuffer.h"

#include <thread>

using namespace std::literals;

int
main()
{
  std::cout << "DataBuffer example ----------" << std::endl;
  Chimia::DiskOffloadableDataBuffer buffer("./");

  int i = 45;
  char c = 'C';
  char k = 'K';

  buffer.Append(&i, sizeof(i));
  buffer.Append(&c, sizeof(c));
  buffer.Append(&k, sizeof(k));

  buffer.Offload();

  std::cout << "Buffer size after offloading: " << buffer.GetSize() << "\n";

  std::this_thread::sleep_for(5s);

  buffer.PullBack();

  std::cout << "Buffer size after pulling back: " << buffer.GetSize() << "\n";

  for (int i = 0; i < buffer.GetSize(); ++i) {
    printf("[%d] = %c\n", i, buffer.GetData()[i]);
  }

  return 0;
}