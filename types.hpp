#ifndef TYPES_HPP
#define TYPES_HPP

#include<set>
#include<string>

using Time = unsigned int;

struct Cube {
  unsigned int attemptsDone = 0;
  const unsigned int maxAttempts;
  const Time solvingTime;
  Cube(Time t, unsigned int max) : maxAttempts(max), solvingTime(t) {};
  bool operator<(const Cube &r) const;
  std::string toString() const;
};

struct CubeCompare {
  bool operator()(const Cube * c1, const Cube * c2) const;
};

enum EventType {
#define SIMU_EVENT_TYPE(Name) \
  Name,
#include "types.def"
  EventUnknown,
};

std::string getEventTypeName(EventType t);

struct Event {
  EventType type;
  Cube *c;
  unsigned int time;
  std::string toString() const;
  bool operator<(const Event &r) const;
};

using EventSet = std::multiset<Event>;

EventSet::iterator findFirst(const EventSet &set, EventType t);

struct Judge {
  unsigned int busyUntil;
  unsigned int idleTime;
  std::string toString() const;
  bool operator<(const Judge &r) const;
};

using CubeSet = std::set<Cube *>;
using SortedCubeSet = std::multiset<Cube *, CubeCompare>;
using JudgeSet = std::multiset<Judge>;


#endif
