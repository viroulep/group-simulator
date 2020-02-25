#include <llvm/ADT/SmallVector.h>
#include <llvm/Support/YAMLTraits.h>
#include "llvm/Support/CommandLine.h"
#include <iostream>
#include <cassert>
#include <iterator>

#include "libsimu.hpp"

using namespace llvm;

static cl::opt<std::string> CostsModelPath("mcosts", cl::desc("Specify path to costs model"), cl::value_desc("filename"));
static cl::opt<std::string> ScramblingCostsPath("scosts", cl::desc("Specify path to scrambling costs"), cl::value_desc("filename"));

int main(int argc, char **argv) {
  cl::ParseCommandLineOptions(argc, argv);
  llvm::SmallVector<int, 4> toto;

  if (CostsModelPath.length() > 0) {
    LoadModelCosts(CostsModelPath);
  }
  if (ScramblingCostsPath.length() > 0) {
    LoadScramblingCost(ScramblingCostsPath);
  }

  toto.push_back(4);
  std::cout << "coucou" << toto.front() << "\n";

  EmitConfig(std::cout);

  return 0;
}
