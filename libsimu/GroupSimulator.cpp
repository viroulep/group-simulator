#include <tuple>
#include <iostream>

#include "GroupSimulator.hpp"
#include "Config.hpp"

using namespace std;

namespace libsimu {

bool Judge::operator<(const Judge &r) const
{
  return busyUntil < r.busyUntil;
}

GroupSimulator::GroupSimulator(WCAEvent &E, const std::vector<Time> &RefTimes) : E(E)
{
  for (Time T : RefTimes) {
    unique_ptr<Cube> C = make_unique<Cube>(T);
    PendingScramble.insert(C.get());
    ActiveCubes.insert(std::move(C));
  }
  Model &MC = Model::get();

  // It doesn't matter if we add the shutdown time right now, and it's
  // inconvenient to hook when the group is done.
  Walltime = MC.InitGroup + MC.ShutdownGroup;
}

unique_ptr<GroupSimulator> GroupSimulator::Create(const string &ModelId,
      WCAEvent &E, const vector<Time> &RefTimes)
{
  // We don't use a string switch here, because using it actually *creates*
  // all unique_ptr before choosing the right one!
  if (ModelId == "Runners") {
    return make_unique<RunnerSystemSimulator>(E, RefTimes);
  } else if (ModelId == "JudgesRun") {
    return make_unique<JudgesRunSimulator>(E, RefTimes);
  } else {
    return unique_ptr<GroupSimulator>{};
  }
}

bool GroupSimulator::ModelUsesRunners(const string &ModelId)
{
  // Most likely having only a couple of simulator kind doesn't deserve genericity.
  return llvm::StringSwitch<bool>(ModelId)
    .Case("Runners", true)
    .Case("JudgesRun", false)
    .Default(false);
}

Time GroupSimulator::Run()
{
  while (!Done()) {
    //cout << *Simu;
    //simu->printState();
    SimuEvent currentEvent = NextEvent();
    if (currentEvent.c) {
      assert(currentEvent.c->AttemptsDone >= 0 && currentEvent.c->AttemptsDone < 5);
    }
    switch (currentEvent.Kind) {
#define SIMU_EVENT_TYPE(Name)            \
      case SimuEvent::Name:                         \
        ActOn##Name(currentEvent); \
      break;
#include "types.def"
      case SimuEvent::Unknown:
        // FIXME: return error!
        cout << "WTF!\n";
    }
    DoneEvent(currentEvent);
  }
  return Walltime;
}

bool SimuEvent::operator<(const SimuEvent &r) const
{
  return tie(T, Kind) < tie(r.T, r.Kind);
}

string SimuEvent::getEventTypeName(EventKind K)
{
  switch(K) {
    case Unknown:
      break;
#define SIMU_EVENT_TYPE(Name) \
    case Name:                \
      return #Name;
#include "types.def"
  }
  return "Unknown";
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
