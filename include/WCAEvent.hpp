#ifndef WCA_EVENTS_HPP
#define WCA_EVENTS_HPP

#include <memory>
#include "libsimu.hpp"

struct WCAEvent {
  enum WCAEventKind {
#define EVENT(Id, Name, MaxAttempts, CutoffAttempts, Rank) \
    E_##Id,
#include "events.def"
    E_Unknown,
  };

  WCAEvent(WCAEventKind K, uint8_t Max, uint8_t Cutoff, uint8_t Rank);
  Time ScramblingCost() const;
  uint8_t MaxAttempts;
  uint8_t CutoffAttempts;
  // AFAIK it's under 255
  uint8_t Rank;
  static WCAEvent *Get(WCAEventKind K);
  static inline std::string WCAEventKindToId(WCAEvent::WCAEventKind K);
private:
  const WCAEventKind Kind;
};




#define EVENT(Id, Name, MaxAttempts, CutoffAttempts, Rank) \
struct WCAEvent##Id : public WCAEvent { \
  WCAEvent##Id() : WCAEvent(E_##Id, MaxAttempts, CutoffAttempts, Rank) {} \
};
#include "events.def"

#endif
