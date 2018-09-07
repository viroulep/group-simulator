#include "types.hpp"
#include "simu.hpp"
#include <iostream>
#include <cassert>
#include <iterator>

using namespace std;

// FIXME cleanup
template<typename C>
void printCollection(const C &s)
{
  cout << "[";
  for (const auto &elem : s)
    cout << elem.toString() << ", ";
  cout << "]";
}

void printCollection(const SortedCubeSet &s);
void printCollection(const CubeSet &s);


void JudgeRunsSystemSimulator::printState() const
{
  cout << "Simulator state:\n";
  cout << "walltime: " << walltime_ << "\n";
  cout << "activeCubes: ";
  printCollection(activeCubes_);
  cout << "\n";
  cout << "idle scramblers: " << scramblersIdle_ << "\n";
  cout << "idle judges: " << judgesIdle_ << "\n";
  cout << "pendingScramble: ";
  printCollection(pendingScramble_);
  cout << "\n";
  cout << "pendingJudging: ";
  printCollection(pendingJudging_);
  cout << "\n";
  cout << "remaining events: ";
  printCollection(events_);
  cout << "\n";
  cout << "----------------------\n";
}

JudgeRunsSystemSimulator::JudgeRunsSystemSimulator(WCAEventKind k, const CubeSet &cubes, unsigned int judges, unsigned int scramblers, Time cutoff, Time timeLimit) : GroupSimulator(k, cubes, cutoff, timeLimit), JudgeRunsSystemCosts("../costs/models.yml")
{
  walltime_ = getInitCost();

  copy(activeCubes_.begin(), activeCubes_.end(), inserter(pendingScramble_, pendingScramble_.begin()));

  for (unsigned int i = 0; i < scramblers; i++) {
    events_.insert(Event{ScramblerReady, nullptr, walltime_});
  }

  for (unsigned int i = 0; i < judges; i++) {
    events_.insert(Event{RunInReady, nullptr, walltime_});
  }
}

void JudgeRunsSystemSimulator::actOnCubeScrambled(const Event &e)
{
  assert(e.c);
  pendingJudging_.insert(e.c);
  if (judgesIdle_) {
    judgesIdle_--;
    events_.insert(Event({RunInReady, nullptr, walltime_}));
  }
}

void JudgeRunsSystemSimulator::actOnCubeSolved(const Event &)
{
  // Not used
  assert(false);
}

void JudgeRunsSystemSimulator::actOnScramblerReady(const Event &)
{
  if (!pendingScramble_.empty()) {
    Cube *c = *pendingScramble_.begin();
    Time doneScrambling = walltime_ + getCostFor(eventForGroup_);
    pendingScramble_.erase(pendingScramble_.begin());
    events_.insert(Event({ScramblerReady, nullptr, doneScrambling}));
    events_.insert(Event({CubeScrambled, c, doneScrambling}));
  } else {
    // Go idle, we'll be waken up by after a run-out
    scramblersIdle_++;
  }
}

void JudgeRunsSystemSimulator::actOnRunInReady(const Event &)
{
  if (!pendingJudging_.empty()) {
    Cube *c = *pendingJudging_.begin();
    pendingJudging_.erase(pendingJudging_.begin());
    Time ranOutTime = walltime_ + getRunInCost() + getJudgingCost() + c->solvingTime + getRunOutCost();
    c->attemptsDone++;
    if (c->attemptsDone == getMaxAttemptsFor(eventForGroup_) ||
        (c->attemptsDone == attemptsForCutoff_ && c->solvingTime >= cutoff_)) {
      assert(activeCubes_.erase(c) == 1);
      delete c;
    } else {
      // We know the judge is going to run it out ASAP
      events_.insert(Event({CubeRanOut, c, ranOutTime}));
    }
    events_.insert(Event({RunInReady, nullptr, ranOutTime}));
  } else {
    judgesIdle_++;
  }
}

void JudgeRunsSystemSimulator::actOnRunOutReady(const Event &)
{
  // Not used
  assert(false);
}

void JudgeRunsSystemSimulator::actOnCubeRanOut(const Event &e)
{
  assert(e.c);
  pendingScramble_.insert(e.c);
  if (scramblersIdle_) {
    scramblersIdle_--;
    events_.insert(Event({ScramblerReady, nullptr, walltime_}));
  }
}
