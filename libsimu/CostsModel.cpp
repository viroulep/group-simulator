#include "libsimu.hpp"
#include "CostsModel.hpp"

using namespace std;
using namespace llvm;

CostsModel *CostsModel::get()
{
  static CostsModel CM;
  return &CM;
}

int LoadCostsModel(const string &Filename)
{
  ErrorOr<unique_ptr<MemoryBuffer>> Buff = MemoryBuffer::getFile(Filename);
  if (!Buff) {
    cerr << "TODO\n";
    return 1;
  }
  yaml::Input YIn(Buff.get()->getBuffer());
  YIn >> *CostsModel::get();
  return 0;
}
