#include "libsimu.hpp"
#include "Config.hpp"

using namespace std;
using namespace llvm;

namespace libsimu {
  Setup &Setup::get() {
    static Setup C;
    return C;
  }

  Model &Model::get()
  {
    static Model MC;
    return MC;
  }

  Scrambling::Scrambling() {
#define EVENT(Id, Name, MaxAttempts, CutoffAttempts, Rank, DefaultScramblingTime) \
    (*this)[#Id] = DefaultScramblingTime;
#include "events.def"
  }

  Scrambling &Scrambling::get()
  {
    static Scrambling SC;
    return SC;
  }

  error_code LoadConfig(const string &Filename)
  {
    ErrorOr<unique_ptr<MemoryBuffer>> Buff = MemoryBuffer::getFile(Filename);
    if (!Buff) {
      return Buff.getError();
    }
    yaml::Input YIn(Buff.get()->getBuffer());
    Config C;
    YIn >> C;
    return YIn.error();
  }
}
