#include <tuple>

#include "GroupSimulator.hpp"
#include "Costs.hpp"

using namespace std;

namespace libsimu {

Cube::Cube(Time T) : SolvingTime(T), Prio(Config::MaxTimeLimit - T) {};

GroupSimulator::GroupSimulator(WCAEvent &E, const CubeSet &CS) :
  E(E), ActiveCubes(CS) {}

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

bool Cube::operator<(const Cube &C) const
{
  // FIXME: improve
  return tie(AttemptsDone, Prio) < tie(C.AttemptsDone, C.Prio);
}

bool CubeCompare::operator()(const Cube * c1, const Cube * c2) const
{
  return *c1 < *c2;
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



RunnerSystemSimulator::RunnerSystemSimulator(WCAEvent &E, const CubeSet &CS) : GroupSimulator(E, CS)
{
  copy(ActiveCubes.begin(), ActiveCubes.end(), inserter(PendingScramble, PendingScramble.begin()));

  Walltime = ModelCosts::get().InitGroup;

  Config &C = Config::get();

  for (unsigned int i = 0; i < C.Judges; i++) {
    Judges.insert(Judge{0, 0});
  }

  for (unsigned int i = 0; i < C.Scramblers; i++) {
    Events.insert(SimuEvent{SimuEvent::ScramblerReady, nullptr, Walltime});
  }

  for (unsigned int i = 0; i < C.Runners; i++) {
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
  Config &C = Config::get();
  e.c->AttemptsDone++;
  cout << "Attempts Done: " << e.c->AttemptsDone << " vs " << to_string(E.MaxAttempts) << "\n";
  if (e.c->AttemptsDone == E.MaxAttempts ||
      (e.c->AttemptsDone == E.CutoffAttempts && e.c->SolvingTime >= C.Cutoff)) {
    size_t Removed = ActiveCubes.erase(e.c);
    assert(Removed == 1);
    delete e.c;
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
  ModelCosts &MC = ModelCosts::get();
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
    Time lastAvailable = Walltime;
    while (!ScrambledCubes.empty() && ++total <= Runner::MaxCubes) {
      Cube *c = *ScrambledCubes.begin();
      ScrambledCubes.erase(ScrambledCubes.begin());
      Judge j = *Judges.begin();
      Judges.erase(Judges.begin());
      // In order to now when we are available to run out, store the last
      // judge availability.
      // If the judge was idle until now, take now as its availability.
      if (j.busyUntil < Walltime) {
        // Register how much time the judge has been idle
        j.idleTime += Walltime - j.busyUntil;
      } else {
        lastAvailable = j.busyUntil;
      }
      // Compute attempt endtime
      j.busyUntil = lastAvailable + MC.CompetitorReady + MC.CompetitorCleanup + c->SolvingTime;
      Judges.insert(j);
      Events.insert(SimuEvent({SimuEvent::CubeSolved, c, j.busyUntil}));
    }
    Events.insert(SimuEvent({SimuEvent::RunnerOutReady, nullptr, lastAvailable}));
  }
}

void RunnerSystemSimulator::ActOnRunnerOutReady(const SimuEvent &)
{
  Time ranOutTime = Walltime + ModelCosts::get().RunOut;
  Events.insert(SimuEvent({SimuEvent::RunnerInReady, nullptr, ranOutTime}));
  cout << "Inserting runin ready @ " << ranOutTime << "\n";
  // Assume a runner can take back all!
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
