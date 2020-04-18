#include "RNG.hpp"

namespace libsimu {
RNG &RNG::get() {
  static RNG C;
  return C;
}

bool RNG::shouldHappen(uint8_t Rate)
{
  return PercentDistrib(R) <= Rate;
}
} // libsimu
