#include <map>
#include "Costs.hpp"
#include "WCAEvent.hpp"

using namespace std;

namespace {
  map<WCAEvent::WCAEventKind, WCAEvent *> WCAEventMap;
}

string WCAEvent::WCAEventKindToId(WCAEventKind K)
{
  switch (K) {
#define EVENT(Id, Name, MaxAttempts, CutoffAttempts,  Rank) \
    case E_##Id: \
      return #Id;
#include "events.def"
    case E_Unknown:
      return "Unknown";
  }
}

Time WCAEvent::ScramblingCost() const
{
  return (*ScramblingCosts::get())[WCAEventKindToId(Kind)];
}

WCAEvent *WCAEvent::Get(WCAEventKind K)
{
  return nullptr;
}
