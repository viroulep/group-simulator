#include "types.hpp"
#include <tuple>
#include <cassert>
#include <algorithm>

using namespace std;

bool Cube::operator<(const Cube &r) const
{
  return attemptsDone < r.attemptsDone;
}

string Cube::toString() const
{
  return "Cube{" + to_string(attemptsDone) + "}";
}

bool CubeCompare::operator()(const Cube * c1, const Cube * c2) const
{
  return *c1 < *c2;
}

string getEventTypeName(EventType t)
{
  switch(t) {
    case EventUnknown:
      return "Unknown";
#define SIMU_EVENT_TYPE(Name) \
    case Name:                \
      return #Name;
#include "types.def"
  }
  // unreachable
  assert(false);
  return "";
}

bool Event::operator<(const Event &r) const
{
  return tie(time, type) < tie(r.time, r.type);
}


string Event::toString() const
{
  string cubeAsString = c ? c->toString() : "NONE";
  return "Ev{" + getEventTypeName(type) + ", " + cubeAsString + ", " + to_string(time) + "}";
}

EventSet::iterator findFirst(const EventSet &set, EventType t)
{
  return find_if(set.begin(), set.end(), [&](const Event &e) {
        return e.type == t;
      });
}

bool Judge::operator<(const Judge &r) const
{
  return busyUntil < r.busyUntil;
}

string Judge::toString() const
{
  return "Judge{busyUntil: " + to_string(busyUntil) + ", idle: " + to_string(idleTime) + "}";
}
