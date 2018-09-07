#include "types.hpp"
#include "simu.hpp"
#include <iostream>
#include <cassert>
#include <iterator>

using namespace std;

//FIXME: extract this so that it's relative to build dir
GroupSimulator::GroupSimulator(WCAEventKind k, const CubeSet &cubes, Time cutoff, Time timeLimit) : ScramblingCosts("../costs/events.yml"), activeCubes_(cubes), eventForGroup_(k), attemptsForCutoff_(getCutoffAttemptsFor(k)), cutoff_(cutoff), timeLimit_(timeLimit)
{}

Event GroupSimulator::nextEvent()
{
  assert(!events_.empty());
  const Event &e = *events_.begin();
  assert(e.time >= walltime_);
  walltime_ = e.time;
  return e;
}

void GroupSimulator::doneEvent()
{
  events_.erase(events_.begin());
}

bool GroupSimulator::done() const
{
  return activeCubes_.empty();
}
