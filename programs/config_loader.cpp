#include <fstream>
#include <iostream>

#include "config_loader.hpp"

using namespace std;
using namespace llvm;

namespace libsimu::programs {

ConfigRet loadConfig(string const &Filename) {

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

ErrCodeTy loadPsychsheet(string const &Filename, vector<Time> &Times) {
  std::ifstream file(Filename);
  if (!file) {
    cerr << "Could not open the psychsheet!\n";
    return errors::GENERIC;
  }
  while (true) {
    Time t;
    file >> t;
    if (file.fail() && !file.eof()) {
      cerr << "Could not read integer\n";
      return errors::GENERIC;
    } else if (!file.fail()) {
      Times.push_back(t);
    }
    if (file.eof()) {
      break;
    }
  }
  return errors::SUCCESS;
}

}
