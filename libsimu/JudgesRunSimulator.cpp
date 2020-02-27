#include "GroupSimulator.hpp"
#include "Costs.hpp"

using namespace std;

namespace libsimu {

JudgesRunSimulator::JudgesRunSimulator(WCAEvent &E, const vector<Time> &RefTimes) : GroupSimulator(E, RefTimes)
{

  Config &C = Config::get();

  //for (unsigned int i = 0; i < C.Judges; i++) {
    //Judges.insert(Judge{0, 0});
  //}

  for (unsigned int i = 0; i < C.Scramblers; i++) {
    Events.insert(SimuEvent{SimuEvent::ScramblerReady, nullptr, Walltime});
  }

  for (unsigned int i = 0; i < C.Judges; i++) {
    Events.insert(SimuEvent{SimuEvent::RunnerInReady, nullptr, Walltime});
  }
}

void JudgesRunSimulator::ActOnCubeScrambled(const SimuEvent &e)
{
  assert(e.c);
  ScrambledCubes.insert(e.c);
  if (judgesIdle_) {
    judgesIdle_--;
    Events.insert(SimuEvent{SimuEvent::RunnerInReady, nullptr, Walltime});
  }
}

void JudgesRunSimulator::ActOnCubeSolved(const SimuEvent &)
{
  // Not used
  assert(false);
}

void JudgesRunSimulator::ActOnScramblerReady(const SimuEvent &)
{
  if (!PendingScramble.empty()) {
    Cube *c = *PendingScramble.begin();
    Time doneScrambling = Walltime + E.ScramblingCost();
    PendingScramble.erase(PendingScramble.begin());
    Events.insert(SimuEvent{SimuEvent::ScramblerReady, nullptr, doneScrambling});
    Events.insert(SimuEvent{SimuEvent::CubeScrambled, c, doneScrambling});
  } else {
    // Go idle, we'll be waken up by after a run-out
    ScramblersAvailable++;
  }
}

void JudgesRunSimulator::ActOnRunnerInReady(const SimuEvent &)
{
  Config &C = Config::get();
  ModelCosts &MC = ModelCosts::get();
  if (!ScrambledCubes.empty()) {
    Cube *c = *ScrambledCubes.begin();
    ScrambledCubes.erase(ScrambledCubes.begin());
    Time ranOutTime = Walltime + MC.RunIn + MC.CompetitorReady + MC.CompetitorCleanup
      + c->SolvingTime + MC.RunOut;
    c->AttemptsDone++;
    if (c->AttemptsDone == E.MaxAttempts ||
        (c->AttemptsDone == E.CutoffAttempts && c->SolvingTime >= C.Cutoff)) {
      ActiveCubes.erase(find_if(ActiveCubes.begin(), ActiveCubes.end(),
            [&](const unique_ptr<Cube> &CP) { return CP.get() == c; }));
    } else {
      // We know the judge is going to run it out ASAP
      Events.insert(SimuEvent{SimuEvent::CubeRanOut, c, ranOutTime});
    }
    Events.insert(SimuEvent{SimuEvent::RunnerInReady, nullptr, ranOutTime});
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
    Events.insert(SimuEvent{SimuEvent::ScramblerReady, nullptr, Walltime});
  }
}

}
