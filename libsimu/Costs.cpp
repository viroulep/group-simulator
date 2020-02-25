#include "libsimu.hpp"
#include "Costs.hpp"
#include "WCAEvent.hpp"

using namespace std;
using namespace llvm;

ModelCosts *ModelCosts::get()
{
  static ModelCosts MC;
  return &MC;
}

ScramblingCosts::ScramblingCosts() {
#define EVENT(Id, Name, MaxAttempts, CutoffAttempts, Rank) \
  (*this)[#Id] = 0;
#include "events.def"
}

ScramblingCosts *ScramblingCosts::get()
{
  static ScramblingCosts SC;
  return &SC;
}

int LoadModelCosts(const string &Filename)
{
  ErrorOr<unique_ptr<MemoryBuffer>> Buff = MemoryBuffer::getFile(Filename);
  if (!Buff) {
    cerr << "TODO\n";
    return 1;
  }
  yaml::Input YIn(Buff.get()->getBuffer());
  YIn >> *ModelCosts::get();
  return 0;
}

int LoadScramblingCost(const string &Filename)
{
  ErrorOr<unique_ptr<MemoryBuffer>> Buff = MemoryBuffer::getFile(Filename);
  if (!Buff) {
    cerr << "TODO\n";
    return 1;
  }
  yaml::Input YIn(Buff.get()->getBuffer());
  YIn >> *ScramblingCosts::get();
  return 0;
}
