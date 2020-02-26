#include <llvm/ADT/SmallVector.h>
#include <llvm/Support/YAMLTraits.h>
#include "llvm/Support/CommandLine.h"
#include <iostream>
#include <cassert>
#include <iterator>

#include "libsimu.hpp"

using namespace llvm;
using namespace libsimu;

static cl::opt<std::string> CostsModelPath("mcosts", cl::desc("Specify path to costs model"), cl::value_desc("filename"));
static cl::opt<std::string> ScramblingCostsPath("scosts", cl::desc("Specify path to scrambling costs"), cl::value_desc("filename"));
static cl::opt<std::string> ConfigPath("config", cl::desc("Specify path to config"), cl::value_desc("filename"));

int main(int argc, char **argv) {
  cl::ParseCommandLineOptions(argc, argv);

  if (CostsModelPath.length() > 0) {
    if (auto err = LoadModelCosts(CostsModelPath)) {
      errs() << "Error loading model cost\n";
      return err.value();
    }
  }
  if (ScramblingCostsPath.length() > 0) {
    if (auto err = LoadScramblingCost(ScramblingCostsPath)) {
      errs() << "Error loading scrambling cost\n";
      return err.value();
    }
  }
  if (ConfigPath.length() > 0) {
    if (auto err = LoadConfig(ConfigPath)) {
      errs() << "Error loading config\n";
      return err.value();
    }
  }


  EmitConfig(std::cout);

  SimuGroup("777", 20);

  TestStuff();


  return 0;
}
