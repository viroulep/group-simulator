#include <cassert>
#include <algorithm>

#include "GroupSimulator.hpp"
#include "Config.hpp"

using namespace std;

namespace libsimu {

JudgesRunSimulator::JudgesRunSimulator(WCAEvent &E,
    vector<Time> const &RefTimes, PropertiesMap const &SetupOverride)
  : GroupSimulator(E, RefTimes, SetupOverride)
{

  for (unsigned int i = 0; i < LocalSetup.Scramblers; i++) {
    Events.insert({SimuEvent::ScramblerReady, nullptr, Walltime});
  }

  for (unsigned int i = 0; i < LocalSetup.Judges; i++) {
    Events.insert({SimuEvent::RunnerInReady, nullptr, Walltime});
  }
}

void JudgesRunSimulator::ActOnCubeScrambled(const SimuEvent &e)
{
  assert(e.c);
  ScrambledCubes.insert(e.c);
  if (judgesIdle_) {
    judgesIdle_--;
    Events.insert({SimuEvent::RunnerInReady, nullptr, Walltime});
  }
}

void JudgesRunSimulator::ActOnCubeSolved(const SimuEvent &e)
{
  assert(e.c);
  Cube *c = e.c;
  c->AttemptsDone++;
  if (CubeIsDone(e.c)) {
    ActiveCubes.erase(find_if(ActiveCubes.begin(), ActiveCubes.end(),
          [&](const unique_ptr<Cube> &CP) { return CP.get() == c; }));
  } else {
    // We know the judge is going to run it out ASAP
    Events.insert({SimuEvent::CubeRanOut, c, e.T});
  }
}

void JudgesRunSimulator::ActOnScramblerReady(const SimuEvent &e)
{
  GroupSimulator::ActOnScramblerReady(e);
}

void JudgesRunSimulator::ActOnRunnerInReady(const SimuEvent &)
{
  Model &MC = Model::get();
  if (!ScrambledCubes.empty()) {
    Cube *c = *ScrambledCubes.begin();
    ScrambledCubes.erase(ScrambledCubes.begin());
    Time ranOutTime = Walltime + MC.RunIn + MC.CompetitorReady + MC.CompetitorCleanup
      + c->getSolvingTime() + MC.RunOut;
    Events.insert(getSolvedWithExtra(c, ranOutTime));
    Events.insert({SimuEvent::RunnerInReady, nullptr, ranOutTime});
  } else {
    // TODO: be smart and insert RunnerInReady later, looking up for a scrambled cube
    // This will let us get rid of this variable!
    judgesIdle_++;
  }
}

void JudgesRunSimulator::ActOnRunnerOutReady(const SimuEvent &)
{
  // Not used
  assert(false);
}

void JudgesRunSimulator::ActOnCubeRanOut(const SimuEvent &e)
{
  assert(e.c);
  PendingScramble.insert(e.c);
  if (ScramblersAvailable) {
    ScramblersAvailable--;
    Events.insert({SimuEvent::ScramblerReady, nullptr, Walltime});
  }
}

}
