#ifndef CUBE_HPP
#define CUBE_HPP

#include "libsimu.hpp"
#include "WCAEvent.hpp"

namespace libsimu {

struct Cube {
  Cube(WCAEvent const &E, Time T, Time TL);
  uint8_t AttemptsDone = 0;
  // This attribute is used for sorting!
  // The slowest the cuber, the highest priority they are
  const Time Prio;
  // Compute the solving time for the AttemptsDone+1 solve.
  Time getSolvingTime() const;
  bool operator<(const Cube &r) const;
  friend std::ostream &operator<<(std::ostream &out, const Cube &C);
private:
  const Time SolvingTime;
  const Time TimeLimit;
  WCAEvent const &Event;
};

struct CubeCompare {
  bool operator()(const Cube * c1, const Cube * c2) const;
};

}

#endif
