#include "config_loader.hpp"

using namespace std;
using namespace llvm;

namespace libsimu::programs {

ConfigRet LoadConfig(const string &Filename) {

  ErrorOr<unique_ptr<MemoryBuffer>> Buff = MemoryBuffer::getFile(Filename);
  if (!Buff) {
    return {Buff.getError().value(), {}};
  }
  yaml::Input YIn(Buff.get()->getBuffer());
  ConfigRet CR;
  YIn >> CR.C;
  CR.Err = YIn.error().value();
  return CR;
}

}
