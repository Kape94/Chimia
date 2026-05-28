#include <iostream>

#include "Core/DataBuffer.h"

int
main()
{
  std::cout << "DataBuffer example ----------" << std::endl;
  Chimia::DataBuffer buffer;

  int i = 45;
  char c = 'C';
  char k = 'K';

  buffer.Append(&i, sizeof(i));
  buffer.Append(&c, sizeof(c));
  buffer.Append(&k, sizeof(k));

  for (int i = 0; i < buffer.GetSize(); ++i) {
    printf("[%d] = %c\n", i, buffer.GetData()[i]);
  }

  return 0;
}