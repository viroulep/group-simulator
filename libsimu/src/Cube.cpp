#include <tuple>
#include "Config.hpp"
#include "Cube.hpp"

using namespace std;

namespace libsimu {

Cube::Cube(const WCAEvent &E, Time T, Time TL) :
  Prio(T > TL ? 0 : TL - T), SolvingTime(T), TimeLimit(TL), Event(E) {};

Time Cube::getSolvingTime() const
{
  // Here we can't just update AttemptDone: the simulation will decide if
  // we get an extra or not.
  if (Event.hasCumulativeTimeLimit()) {
    Time Consumed = AttemptsDone * SolvingTime;
    // If we're over the cumulative, remaining attempts are DNS!
    if (Consumed > TimeLimit) {
      return 0;
    } else {
      Time Remaining = TimeLimit - Consumed;
      return SolvingTime > Remaining ? Remaining : SolvingTime;
    }
  } else {
    return SolvingTime > TimeLimit ? TimeLimit : SolvingTime;
  }
}

bool Cube::operator<(const Cube &C) const
{
  // FIXME: improve
  return tie(AttemptsDone, Prio) < tie(C.AttemptsDone, C.Prio);
}

bool CubeCompare::operator()(const Cube * c1, const Cube * c2) const
{
  return *c1 < *c2;
}

}
