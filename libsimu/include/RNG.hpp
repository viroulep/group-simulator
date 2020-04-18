#ifndef RNG_HPP
#define RNG_HPP

#include <random>

namespace libsimu {

struct RNG {
  static RNG &get();
  bool shouldHappen(uint8_t Rate);
private:
  RNG() = default;
  std::random_device R;
  std::uniform_int_distribution<uint8_t> PercentDistrib{1, 100};
};

}

#endif
