#ifndef GROUP_SIMULATOR_HPP
#define GROUP_SIMULATOR_HPP

#include <set>
#include "libsimu.hpp"
#include "Actors.hpp"
#include "Cube.hpp"
#include "WCAEvent.hpp"

namespace libsimu {

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


using EventQueue = std::multiset<SimuEvent>;
using JudgeQueue = std::multiset<Judge>;

using SortedCubeSet = std::multiset<Cube *, CubeCompare>;

struct GroupSimulator {
public:
  GroupSimulator(WCAEvent &E, const std::vector<Time> &RefTimes);
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
  // The set of active cubes throughout the simulation.
  // It has ownership of the pointer.
  std::set<std::unique_ptr<Cube>> ActiveCubes;

  // Other (sorted) sets of cubes, they contain only cubes which are still active.
  SortedCubeSet PendingScramble;
  SortedCubeSet ScrambledCubes;
  SortedCubeSet SolvedCubes;
  uint8_t ScramblersAvailable = 0;
};

class RunnerSystemSimulator : public GroupSimulator {
public:
  RunnerSystemSimulator(WCAEvent &E, const std::vector<Time> &RefTimes);
  ~RunnerSystemSimulator() {};
  virtual std::ostream &EmitToStream(std::ostream &out) const override;
#define SIMU_EVENT_TYPE(Name) \
  virtual void ActOn##Name(const SimuEvent &e) override;
#include "types.def"
};

class JudgesRunSimulator : public GroupSimulator {
public:
  JudgesRunSimulator(WCAEvent &E, const std::vector<Time> &RefTimes);
  ~JudgesRunSimulator() {};
  unsigned int judgesIdle_ = 0;
  virtual std::ostream &EmitToStream(std::ostream &out) const override;
#define SIMU_EVENT_TYPE(Name) \
  virtual void ActOn##Name(const SimuEvent &e) override;
#include "types.def"
};



}
#endif
