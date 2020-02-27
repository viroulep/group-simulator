#include <tuple>

#include "GroupSimulator.hpp"
#include "Costs.hpp"

using namespace std;

namespace libsimu {


GroupSimulator::GroupSimulator(WCAEvent &E, const std::vector<Time> &RefTimes) : E(E)
{
  for (Time T : RefTimes) {
    unique_ptr<Cube> C = make_unique<Cube>(T);
    PendingScramble.insert(C.get());
    ActiveCubes.insert(std::move(C));
  }

  Walltime = ModelCosts::get().InitGroup;
}

bool SimuEvent::operator<(const SimuEvent &r) const
{
  return tie(T, Kind) < tie(r.T, r.Kind);
}

string SimuEvent::getEventTypeName(EventKind K)
{
  switch(K) {
    case Unknown:
      return "Unknown";
#define SIMU_EVENT_TYPE(Name) \
    case Name:                \
      return #Name;
#include "types.def"
  }
}


SimuEvent GroupSimulator::NextEvent()
{
  assert(!Events.empty());
  const SimuEvent &SE = *Events.begin();
  assert(SE.T >= Walltime);
  Walltime = SE.T;
  Events.erase(Events.begin());
  return SE;
}

void GroupSimulator::DoneEvent(SimuEvent SE)
{
}

bool GroupSimulator::Done() const
{
  return ActiveCubes.empty();
}

EventQueue::iterator GroupSimulator::findFirst(const SimuEvent::EventKind K)
{
  return find_if(Events.begin(), Events.end(), [&](const SimuEvent &SE) {
    return SE.Kind == K;
  });
}


}
