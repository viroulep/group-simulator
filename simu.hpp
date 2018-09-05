#ifndef SIMU_HPP
#define SIMU_HPP

#include "types.hpp"

class GroupSimulator {
protected:
  Time walltime_ = 0;
  EventSet events_;
  JudgeSet judges_;
public:
  Event nextEvent();
  Time walltime() const { return walltime_; };
  virtual void doneEvent();
  // TODO default implem
  virtual void printState() const = 0;
  virtual bool done() const = 0;
#define SIMU_EVENT_TYPE(Name) \
  virtual void actOn##Name(const Event &e) = 0;
#include "types.def"

};

class RunnerSystemSimulator : public GroupSimulator {
  SortedCubeSet pendingScramble_;
  SortedCubeSet pendingRunIn_;
  SortedCubeSet pendingRunOut_;
  unsigned int scramblersIdle_;
  unsigned int activeCubes_;
public:
  RunnerSystemSimulator(unsigned int cubes, unsigned int judges, unsigned int scramblers, unsigned int runners);
  virtual bool done() const;
#define SIMU_EVENT_TYPE(Name) \
  virtual void actOn##Name(const Event &e);
#include "types.def"
  virtual void printState() const;
};

#endif
