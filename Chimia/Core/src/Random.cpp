#include "Random.h"
#include "CoreNamespaceDefs.h"

#include <cstdlib>
#include <ctime>

// ----------------------------------------------------------------------------

USING_CHIMIA_CORE_NAMESPACE

// ----------------------------------------------------------------------------

namespace {
class RandomGenerator
{
public:
  RandomGenerator() { srand(time(nullptr)); }

  float Rand() { return (float)rand() / RAND_MAX; }
};
}

// ----------------------------------------------------------------------------

float
CHIMIA_CORE_NAMESPACE::Rand()
{
  static RandomGenerator generator;
  return generator.Rand();
}

// ----------------------------------------------------------------------------