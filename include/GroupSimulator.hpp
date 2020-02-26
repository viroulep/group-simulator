#ifndef GROUP_SIMULATOR_HPP
#define GROUP_SIMULATOR_HPP

#include <set>
#include "libsimu.hpp"
#include "Actors.hpp"
#include "WCAEvent.hpp"

namespace libsimu {

struct Cube {
  Cube(Time T);
  uint8_t AttemptsDone = 0;
  const Time SolvingTime;
  // This attribute is used for sorting!
  // The slowest the cuber, the highest priority they are
  const Time Prio;
  bool operator<(const Cube &r) const;
  friend std::ostream &operator<<(std::ostream &out, const Cube &C);
};

struct SimuEvent {
  enum EventKind {
#define SIMU_EVENT_TYPE(Name) \
    Name,
#include "types.def"
    Unknown,
  };

  EventKind Kind;
  Cube *c;
  //Judge *c;
  //Runner *c;
  //std::string toString() const;
  Time T;
  bool operator<(const SimuEvent &r) const;
  friend std::ostream &operator<<(std::ostream &out, const SimuEvent &SE);
  static std::string getEventTypeName(EventKind K);
};

struct CubeCompare {
  bool operator()(const Cube * c1, const Cube * c2) const;
};


using EventQueue = std::multiset<SimuEvent>;
using JudgeQueue = std::multiset<Judge>;

using CubeSet = std::set<Cube *>;
using SortedCubeSet = std::multiset<Cube *, CubeCompare>;

struct GroupSimulator {
public:
  GroupSimulator(WCAEvent &E, const CubeSet &cubes);
  virtual ~GroupSimulator() {};
  SimuEvent NextEvent();
  Time GetWalltime() const { return Walltime; };
  virtual void DoneEvent(SimuEvent SE);
  virtual bool Done() const;
#define SIMU_EVENT_TYPE(Name) \
  virtual void ActOn##Name(const SimuEvent &e) = 0;
#include "types.def"
  virtual std::ostream &EmitToStream(std::ostream &out) const = 0;
  friend std::ostream &operator<<(std::ostream &out, const GroupSimulator &GS);

protected:
  EventQueue::iterator findFirst(SimuEvent::EventKind K);
  Time Walltime = 0;
  EventQueue Events;
  JudgeQueue Judges;
  WCAEvent &E;
  CubeSet ActiveCubes;
  SortedCubeSet PendingScramble;
  SortedCubeSet ScrambledCubes;
  SortedCubeSet SolvedCubes;
  uint8_t ScramblersAvailable = 0;
  //const WCAEventKind eventForGroup_;
  //const unsigned int attemptsForCutoff_;
  //const Time cutoff_;
  //const Time timeLimit_;

  // SimuEvents
  // ReadyCubes
  // ScramblersAvailable
  // PendingRunInCubes
  // ReadyJudges
  // PendingRunOutCubes
};

class RunnerSystemSimulator : public GroupSimulator {
public:
  RunnerSystemSimulator(WCAEvent &E, const CubeSet &cubes);
  ~RunnerSystemSimulator() {};
  virtual std::ostream &EmitToStream(std::ostream &out) const override;
#define SIMU_EVENT_TYPE(Name) \
  virtual void ActOn##Name(const SimuEvent &e) override;
#include "types.def"
};



}
#endif
