#ifndef CUBE_HPP
#define CUBE_HPP

#include "libsimu.hpp"

namespace libsimu {

struct Cube {
  Cube(Time T);
  uint8_t AttemptsDone = 0;
  const Time SolvingTime;
  // This attribute is used for sorting!
  // The slowest the cuber, the highest priority they are
  const Time Prio;
  bool operator<(const Cube &r) const;
  friend std::ostream &operator<<(std::ostream &out, const Cube &C);
};

struct CubeCompare {
  bool operator()(const Cube * c1, const Cube * c2) const;
};

}

#endif
