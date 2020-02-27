#include "Actors.hpp"

namespace libsimu {
  bool Judge::operator<(const Judge &r) const
  {
    return busyUntil < r.busyUntil;
  }

  Config &Config::get() {
    static Config C;
    return C;
  }
}
