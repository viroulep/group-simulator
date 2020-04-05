#include "config_loader.hpp"

using namespace std;
using namespace llvm;

namespace libsimu::programs {

ConfigRet loadConfig(const string &Filename) {

  ErrorOr<unique_ptr<MemoryBuffer>> Buff = MemoryBuffer::getFile(Filename);
  if (!Buff) {
    return {errors::INVALID_FILE, {}};
  }
  yaml::Input YIn(Buff.get()->getBuffer());
  ConfigRet CR;
  YIn >> CR.C;
  CR.Err = YIn.error() ? errors::INVALID_CONFIG : errors::SUCCESS;
  return CR;
}

}
