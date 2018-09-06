#ifndef SIMU_HPP
#define SIMU_HPP

#include "types.hpp"
#include "costs.hpp"

class GroupSimulator : public ScramblingCosts {
protected:
  Time walltime_ = 0;
  EventSet events_;
  JudgeSet judges_;
  CubeSet activeCubes_;
  const WCAEventKind eventForGroup_;
  const Time cutoff_;
  const Time timeLimit_;
public:
  GroupSimulator(WCAEventKind k, unsigned int cubes, Time cutoff, Time timeLimit);
  virtual ~GroupSimulator() {};
  Event nextEvent();
  Time walltime() const { return walltime_; };
  virtual void doneEvent();
  // TODO default implem
  virtual void printState() const = 0;
  virtual bool done() const;
#define SIMU_EVENT_TYPE(Name) \
  virtual void actOn##Name(const Event &e) = 0;
#include "types.def"

};

class RunnerSystemSimulator : public GroupSimulator, public RunnerSystemCosts {
  SortedCubeSet pendingScramble_;
  SortedCubeSet pendingRunIn_;
  SortedCubeSet pendingRunOut_;
  unsigned int scramblersIdle_;
public:
  RunnerSystemSimulator(WCAEventKind k, unsigned int cubes, unsigned int judges, unsigned int scramblers, unsigned int runners, Time cutoff, Time timeLimit);
  ~RunnerSystemSimulator() {};
#define SIMU_EVENT_TYPE(Name) \
  virtual void actOn##Name(const Event &e);
#include "types.def"
  virtual void printState() const;
};

#endif
