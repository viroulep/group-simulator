#include "libsimu.hpp"
#include "CostsModel.hpp"

using namespace std;

void EmitConfig(ostream &out) {
  out << *CostsModel::get();
}

ostream &operator<<(ostream &out, const CostsModel &CM)
{
  out << "CostsModel {\n";
  out << "  InitGroup: " << CM.InitGroup << "\n";
  out << "  RunIn: " << CM.RunIn << "\n";
  out << "  CompetitorReady: " << CM.CompetitorReady << "\n";
  out << "  CompetitorCleanup: " << CM.CompetitorCleanup << "\n";
  out << "  RunOut: " << CM.RunOut << "\n";
  out << "  ShutdownGroup: " << CM.ShutdownGroup << "\n";
  out << "  CubesPerRunner: " << CM.CubesPerRunner << "\n";
  out << "}\n";
  return out;
}
