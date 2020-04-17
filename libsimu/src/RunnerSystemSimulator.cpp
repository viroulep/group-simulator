#include <cassert>
#include <algorithm>

#include "Config.hpp"
#include "GroupSimulator.hpp"

using namespace std;

namespace libsimu {

RunnerSystemSimulator::RunnerSystemSimulator(WCAEvent &E,
    vector<Time> const &RefTimes, PropertiesMap const &SetupOverride)
  : GroupSimulator(E, RefTimes, SetupOverride)
{

  for (unsigned int i = 0; i < LocalSetup.Judges; i++) {
    Judges.insert(Judge{0, 0});
  }

  for (unsigned int i = 0; i < LocalSetup.Scramblers; i++) {
    Events.insert(SimuEvent{SimuEvent::ScramblerReady, nullptr, Walltime});
  }

  for (unsigned int i = 0; i < LocalSetup.Runners; i++) {
    Events.insert(SimuEvent{SimuEvent::RunnerInReady, nullptr, Walltime});
  }
}

void RunnerSystemSimulator::ActOnCubeScrambled(const SimuEvent &e)
{
  assert(e.c);
  ScrambledCubes.insert(e.c);
}

void RunnerSystemSimulator::ActOnCubeSolved(const SimuEvent &e)
{
  assert(e.c);
  e.c->AttemptsDone++;
  if (e.c->AttemptsDone == E.MaxAttempts ||
      (e.c->AttemptsDone == E.CutoffAttempts
       && e.c->SolvingTime >= LocalSetup.Cutoff)) {
    ActiveCubes.erase(find_if(ActiveCubes.begin(), ActiveCubes.end(),
          [&](const unique_ptr<Cube> &CP) { return CP.get() == e.c; }));
  } else {
    SolvedCubes.insert(e.c);
  }
}

void RunnerSystemSimulator::ActOnScramblerReady(const SimuEvent &)
{
  if (!PendingScramble.empty()) {
    Cube *c = *PendingScramble.begin();
    Time doneScrambling = Walltime + E.ScramblingCost();
    PendingScramble.erase(PendingScramble.begin());
    Events.insert(SimuEvent({SimuEvent::ScramblerReady, nullptr, doneScrambling}));
    Events.insert(SimuEvent({SimuEvent::CubeScrambled, c, doneScrambling}));
  } else {
    // Go idle, we'll be waken up by after a run-out
    ScramblersAvailable++;
  }
}

void RunnerSystemSimulator::ActOnRunnerInReady(const SimuEvent &)
{
  Model &MC = Model::get();
  if (ScrambledCubes.empty()) {
    // Else try to run out a few seconds later
    if (!SolvedCubes.empty()) {
      Events.insert(SimuEvent({SimuEvent::RunnerOutReady, nullptr, Walltime + MC.RunIn}));
    } else {
      // Attempt to find a scrambled cube in the queue
      auto nextEvent = findFirst(SimuEvent::CubeScrambled);
      if (nextEvent != Events.end()) {
        Events.insert(SimuEvent({SimuEvent::RunnerInReady, nullptr, nextEvent->T}));
      } else {
        nextEvent = findFirst(SimuEvent::CubeSolved);
        if (nextEvent != Events.end()) {
          Events.insert(SimuEvent({SimuEvent::RunnerOutReady, nullptr, nextEvent->T}));
        } else {
          // Else just wait an arbitrary amount of time
          Events.insert(SimuEvent({SimuEvent::RunnerInReady, nullptr, Walltime + 10}));
        }
      }
    }
  } else {
    unsigned int total = 0;
    // The runner needs to run in the cubes, then it will be available to run out.
    Time NextRunnerAvailability = Walltime + MC.RunIn;
    while (!ScrambledCubes.empty() && ++total <= LocalSetup.MaxCubes) {
      Cube *c = *ScrambledCubes.begin();
      ScrambledCubes.erase(ScrambledCubes.begin());
      Judge j = *Judges.begin();
      Judges.erase(Judges.begin());
      // In order to now when we are available to run out, store the last
      // judge availability (we may be looking at the future by not checking
      // for judge availability at the current time!).
      // If the judge was idle until now, take now as its availability.
      if (j.busyUntil < NextRunnerAvailability) {
        // Register how much time the judge has been idle
        j.idleTime += NextRunnerAvailability - j.busyUntil;
      } else {
        NextRunnerAvailability = j.busyUntil;
      }
      // Compute attempt endtime
      j.busyUntil = NextRunnerAvailability + MC.CompetitorReady + MC.CompetitorCleanup + c->SolvingTime;
      Judges.insert(j);
      Events.insert(SimuEvent({SimuEvent::CubeSolved, c, j.busyUntil}));
    }
    Events.insert(SimuEvent({SimuEvent::RunnerOutReady, nullptr, NextRunnerAvailability}));
  }
}

void RunnerSystemSimulator::ActOnRunnerOutReady(const SimuEvent &)
{
  Time ranOutTime = Walltime + Model::get().RunOut;
  Events.insert(SimuEvent({SimuEvent::RunnerInReady, nullptr, ranOutTime}));
  // Assume a runner can run back any number of cubes!
  while (!SolvedCubes.empty()) {
    Cube *c = *SolvedCubes.begin();
    SolvedCubes.erase(SolvedCubes.begin());
    Events.insert(SimuEvent({SimuEvent::CubeRanOut, c, ranOutTime}));
  }
}

void RunnerSystemSimulator::ActOnCubeRanOut(const SimuEvent &e)
{
  assert(e.c);
  PendingScramble.insert(e.c);
  // Wake up a Scrambler a mark them ready
  if (ScramblersAvailable) {
    ScramblersAvailable--;
    Events.insert(SimuEvent({SimuEvent::ScramblerReady, nullptr, Walltime}));
  }
}

}
