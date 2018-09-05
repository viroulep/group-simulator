#include "types.hpp"
#include "simu.hpp"
#include <iostream>
#include <cassert>

using namespace std;

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

// FIXME cleanup
template<typename C>
void printCollection(const C &s)
{
  cout << "[";
  for (const auto &elem : s)
    cout << elem.toString() << ", ";
  cout << "]";
}

void printCollection(const SortedCubeSet &s)
{
  cout << "[";
  for (const auto &elem : s)
    cout << elem->toString() << ", ";
  cout << "]";
}


void RunnerSystemSimulator::printState() const
{
  cout << "Simulator state:\n";
  cout << "walltime: " << walltime_ << "\n";
  cout << "activeCubes: " << activeCubes_ << "\n";
  cout << "idle scramblers: " << scramblersIdle_ << "\n";
  cout << "pendingScramble: ";
  printCollection(pendingScramble_);
  cout << "\n";
  cout << "pendingRunIn: ";
  printCollection(pendingRunIn_);
  cout << "\n";
  cout << "pendingRunOut: ";
  printCollection(pendingRunOut_);
  cout << "\n";
  cout << "judges: ";
  printCollection(judges_);
  cout << "\n";
  cout << "remaining events: ";
  printCollection(events_);
  cout << "\n";
  cout << "----------------------\n";
}

RunnerSystemSimulator::RunnerSystemSimulator(unsigned int cubes, unsigned int judges, unsigned int scramblers, unsigned int runners) : GroupSimulator(), activeCubes_(cubes)
{
  for (unsigned int i = 0; i < activeCubes_; i++) {
    pendingScramble_.insert(new Cube{0});
  }
  for (unsigned int i = 0; i < judges; i++) {
    judges_.insert(Judge{0, 0});
  }

  for (unsigned int i = 0; i < scramblers; i++) {
    events_.insert(Event{ScramblerReady, nullptr, 0});
  }

  for (unsigned int i = 0; i < runners; i++) {
    events_.insert(Event{RunInReady, nullptr, 0});
  }
}

bool RunnerSystemSimulator::done() const
{
  return !activeCubes_;
}

void RunnerSystemSimulator::actOnCubeScrambled(const Event &e)
{
  assert(e.c);
  pendingRunIn_.insert(e.c);
}

void RunnerSystemSimulator::actOnCubeSolved(const Event &e)
{
  assert(e.c);
  if (e.c->attemptsDone == 4) {
    activeCubes_--;
    delete e.c;
  } else {
    e.c->attemptsDone++;
    cout << "inserting in pending run out: " << e.c->toString() << "\n";
    pendingRunOut_.insert(e.c);
  }
}

void RunnerSystemSimulator::actOnScramblerReady(const Event &)
{
  if (!pendingScramble_.empty()) {
    Cube *c = *pendingScramble_.begin();
    // FIXME: extract 30s to model
    Time doneScrambling = walltime_ + 20;
    pendingScramble_.erase(pendingScramble_.begin());
    events_.insert(Event({ScramblerReady, nullptr, doneScrambling}));
    events_.insert(Event({CubeScrambled, c, doneScrambling}));
  } else {
    // Go idle, we'll be waken up by after a run-out
    scramblersIdle_++;
  }
}

void RunnerSystemSimulator::actOnRunInReady(const Event &)
{
  if (pendingRunIn_.empty()) {
    // Else try to run out a few seconds later
    // FIXME: extract 20s to model
    if (!pendingRunOut_.empty()) {
      events_.insert(Event({RunOutReady, nullptr, walltime_ + 20}));
    } else {
      // Attempt to find a scrambled cube in the queue
      auto nextEvent = findFirst(events_, CubeScrambled);
      if (nextEvent != events_.end()) {
        events_.insert(Event({RunInReady, nullptr, nextEvent->time}));
      } else {
        nextEvent = findFirst(events_, CubeSolved);
        if (nextEvent != events_.end()) {
          events_.insert(Event({RunOutReady, nullptr, nextEvent->time}));
        } else {
          // Else just wait an arbitrary amount of time
          events_.insert(Event({RunInReady, nullptr, walltime_ + 10}));
        }
      }
    }
  } else {
    unsigned int max = 2;
    unsigned int total = 0;
    Time lastAvailable;
    while (!pendingRunIn_.empty() && ++total <= max) {
      Cube *c = *pendingRunIn_.begin();
      pendingRunIn_.erase(pendingRunIn_.begin());
      cout << "Running in: " << c->toString() << "\n";
      Judge j = *judges_.begin();
      judges_.erase(judges_.begin());
      // In order to now when we are available to run out, store the last
      // judge availability.
      // If the judge was idle until now, take now as its availability.
      if (j.busyUntil < walltime_) {
        // Register how much time the judge has been idle
        j.idleTime += walltime_ - j.busyUntil;
        lastAvailable = walltime_;
      } else {
        lastAvailable = j.busyUntil;
      }
      // Compute attempt endtime
      // FIXME: extract 60s to model
      j.busyUntil = lastAvailable + 60;
      judges_.insert(j);
      events_.insert(Event({CubeSolved, c, j.busyUntil}));
    }
    events_.insert(Event({RunOutReady, nullptr, lastAvailable}));
  }
}

void RunnerSystemSimulator::actOnRunOutReady(const Event &)
{
  // FIXME: extract 30s to model
  events_.insert(Event({RunInReady, nullptr, walltime_ + 20}));
  // Assume a runner can take back all!
  while (!pendingRunOut_.empty()) {
    Cube *c = *pendingRunOut_.begin();
    pendingRunOut_.erase(pendingRunOut_.begin());
    events_.insert(Event({CubeRanOut, c, walltime_ + 20}));
  }
}

void RunnerSystemSimulator::actOnCubeRanOut(const Event &e)
{
  assert(e.c);
  pendingScramble_.insert(e.c);
  if (scramblersIdle_) {
    scramblersIdle_--;
    events_.insert(Event({ScramblerReady, nullptr, walltime_}));
  }
}
