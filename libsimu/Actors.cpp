#include "Actors.hpp"

namespace libsimu {
  uint8_t Judge::ExtraRate = 0;
  uint8_t Runner::MaxCubes = 3;
  uint8_t Scrambler::MiscrambleRate = 0;

  bool Judge::operator<(const Judge &r) const
  {
    return busyUntil < r.busyUntil;
  }

  Config &Config::get() {
    static Config C;
    return C;
  }
}
