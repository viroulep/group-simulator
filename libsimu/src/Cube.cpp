#include <tuple>
#include "Config.hpp"
#include "Cube.hpp"

using namespace std;

namespace libsimu {

Cube::Cube(Time T) : SolvingTime(T), Prio(Setup::MaxTimeLimit - T) {};

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
