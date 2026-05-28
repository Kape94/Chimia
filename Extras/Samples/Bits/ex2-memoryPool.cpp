#include <iostream>

#include "Bits/MemoryPool/MemoryPool.h"

class Object
{
public:
  Object() = delete;

  Object(int _n, char _c)
    : n(_n)
    , c(_c)
  {
  }

  void Print() { std::cout << "n = " << n << " c = " << c << std::endl; }

private:
  int n;
  char c;
};

int
main()
{
  Chimia::Bits::MemoryPool<Object> objectsPool(100);

  auto o = objectsPool.New(10, 'c');
  o->Print();
  objectsPool.Release(o);

  return 0;
}