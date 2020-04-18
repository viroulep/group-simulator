#include <algorithm>
#include <cassert>

#include "GroupSimulator.hpp"
#include "Config.hpp"
#include "RNG.hpp"

using namespace std;

namespace libsimu {

bool Judge::operator<(const Judge &r) const
{
  return busyUntil < r.busyUntil;
}

GroupSimulator::GroupSimulator(WCAEvent &E, std::vector<Time> const &RefTimes,
    PropertiesMap const &SetupOverride) : LocalSetup(Setup::cget()), E(E)
{
  for (Time T : RefTimes) {
    unique_ptr<Cube> C = make_unique<Cube>(T);
    PendingScramble.insert(C.get());
    ActiveCubes.insert(std::move(C));
  }
  // Load setup override, if any

  LocalSetup.loadMap(SetupOverride);
  Model &MC = Model::get();

  // It doesn't matter if we add the shutdown time right now, and it's
  // inconvenient to hook when the group is done.
  Walltime = MC.InitGroup + MC.ShutdownGroup;
}

unique_ptr<GroupSimulator> GroupSimulator::Create(string const &ModelId,
      WCAEvent &E, vector<Time> const &RefTimes,
      PropertiesMap const &SetupOverride)
{
  // We don't use a string switch here, because using it actually *creates*
  // all unique_ptr before choosing the right one!
  if (ModelId == "Runners") {
    return make_unique<RunnerSystemSimulator>(E, RefTimes, SetupOverride);
  } else if (ModelId == "JudgesRun") {
    return make_unique<JudgesRunSimulator>(E, RefTimes, SetupOverride);
  } else {
    return unique_ptr<GroupSimulator>{};
  }
}

bool GroupSimulator::ModelUsesRunners(const string &ModelId)
{
  // Most likely having only a couple of simulator kind doesn't deserve genericity.
  if (ModelId == "Runners") {
    return true;
  } else if (ModelId == "JudgesRun") {
    return false;
  }
  return false;
}

SimuEvent GroupSimulator::getSolvedWithExtra(Cube *c, Time t)
{
  if (RNG::get().shouldHappen(LocalSetup.ExtraRate)) {
    t += Model::get().ExtraDelay;
  }
  return { SimuEvent::CubeSolved, c, t };
}

TimeResult GroupSimulator::Run()
{
  while (!Done()) {
    SimuEvent currentEvent = NextEvent();
    if (currentEvent.c
        && (currentEvent.c->AttemptsDone < 0
            || currentEvent.c->AttemptsDone >= 5)) {
      return {errors::SIMULATION_FAILURE, 0};
    }
    switch (currentEvent.Kind) {
#define SIMU_EVENT_TYPE(Name)            \
      case SimuEvent::Name:                         \
        ActOn##Name(currentEvent); \
      break;
#include "types.def"
      case SimuEvent::Unknown:
        return {errors::SIMULATION_FAILURE, 0};
    }
    DoneEvent(currentEvent);
  }
  return {errors::SUCCESS, Walltime};
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
  SimuEvent SE = std::move(*Events.begin());
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

void GroupSimulator::ActOnCubeSolved(const SimuEvent &) {}

void GroupSimulator::ActOnCubeScrambled(const SimuEvent &) {}

void GroupSimulator::ActOnCubeRanOut(const SimuEvent &) {}

void GroupSimulator::ActOnScramblerReady(const SimuEvent &)
{
  if (!PendingScramble.empty()) {
    Cube *c = *PendingScramble.begin();
    Time doneScrambling = Walltime + E.ScramblingCost();
    PendingScramble.erase(PendingScramble.begin());
    if (RNG::get().shouldHappen(LocalSetup.MiscrambleRate)) {
      // Oops, the scrambler made a mistake
      // Assume that it's given back to the competitor, which solves it and
      // give it back.
      // We handle that by inserting a "CubeRanOut" later in the queue.
      Events.insert({SimuEvent::CubeRanOut, c, doneScrambling + c->SolvingTime});
    } else {
      // Proceed normally
      Events.insert({SimuEvent::CubeScrambled, c, doneScrambling});
    }
    Events.insert({SimuEvent::ScramblerReady, nullptr, doneScrambling});
  } else {
    // Go idle, we'll be waken up by after a run-out
    ScramblersAvailable++;
  }
}

void GroupSimulator::ActOnRunnerInReady(const SimuEvent &) {}

void GroupSimulator::ActOnRunnerOutReady(const SimuEvent &) {}

}
