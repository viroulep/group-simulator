#include "libsimu.hpp"
#include "Costs.hpp"

using namespace std;

void EmitConfig(ostream &out) {
  out << *ModelCosts::get();
  out << *ScramblingCosts::get();
}

ostream &operator<<(ostream &out, const ModelCosts &MC)
{
  out << "CostsModel {\n";
  out << "  InitGroup: " << MC.InitGroup << "\n";
  out << "  RunIn: " << MC.RunIn << "\n";
  out << "  CompetitorReady: " << MC.CompetitorReady << "\n";
  out << "  CompetitorCleanup: " << MC.CompetitorCleanup << "\n";
  out << "  RunOut: " << MC.RunOut << "\n";
  out << "  ShutdownGroup: " << MC.ShutdownGroup << "\n";
  out << "  CubesPerRunner: " << MC.CubesPerRunner << "\n";
  out << "}\n";
  return out;
}

ostream &operator<<(ostream &out, const ScramblingCosts &SC)
{
  out << "ScramblingCosts {\n";
  for (auto &entry : SC) {
    out << string("  ") << entry.first().str() << ": " << entry.second << "\n";
  }
  out << "}\n";
  return out;
}
