#include "libsimu.hpp"
#include "Costs.hpp"
#include "Debug.hpp"
#include "Actors.hpp"
#include "GroupSimulator.hpp"

using namespace std;

namespace libsimu {

void EmitConfig(ostream &out) {
  out << ModelCosts::get();
  out << ScramblingCosts::get();
  out << Config::get();
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

ostream &operator<<(ostream &out, const Config &C)
{
  // zext the uint8_t so that it's not considered as a (non-printable) char.
  out << "Config {\n";
  out << "  CubesPerRunner: " << uint64_t{Runner::MaxCubes} << "\n";
  out << "  ExtraRate: " << uint64_t{Judge::ExtraRate} << "\n";
  out << "  MiscrambleRate: " << uint64_t{Scrambler::MiscrambleRate} << "\n";
  out << "  Judges: " << uint64_t{C.Judges} << "\n";
  out << "  Runners: " << uint64_t{C.Runners} << "\n";
  out << "  Scramblers: " << uint64_t{C.Scramblers} << "\n";
  out << "  TimeLimit: " << C.TimeLimit << "\n";
  out << "  Cutoff: " << C.Cutoff << "\n";
  out << "}\n";
  return out;
}

ostream &operator<<(ostream &out, const SimuEvent &SE)
{
  out << "Ev{";
  out << SimuEvent::getEventTypeName(SE.Kind);
  out << ", ";
  if (SE.c)
    out << *SE.c;
  else
    out << "NONE";
  out << ", " << to_string(SE.T) << "}";
  return out;
}

ostream &operator<<(ostream &out, const Cube &C)
{
  out << "Cube{" << to_string(C.AttemptsDone) << "}";
  return out;
}

ostream &operator<<(ostream &out, const Judge &J)
{
  out << "Judge{busyUntil: " << to_string(J.busyUntil);
  out << ", idle: " << to_string(J.idleTime) << "}";
  return out;
}

ostream &operator<<(ostream &out, const GroupSimulator &GS)
{
  return GS.EmitToStream(out);
}

ostream &RunnerSystemSimulator::EmitToStream(ostream &out) const
{
  out << "Simulator {\n";
  out << "  walltime: " << Walltime << "\n";
  out << "  activeCubes: " << ActiveCubes << "\n";
  out << "  idle scramblers: " << ScramblersAvailable << "\n";
  out << "  pendingScramble: " << PendingScramble << "\n";
  out << "  pendingRunIn: " << ScrambledCubes << "\n";
  out << "  pendingRunOut: " << SolvedCubes << "\n";
  out << "  judges: " << Judges << "\n";
  out << "  remaining events: " << Events << "\n";
  out << "----------------------\n";
  return out;
}

ostream &operator<<(ostream &out, const WCAEvent &Ev)
{
  out << "WCAEvent{" << Ev.Id;
  out << ",M:" << to_string(Ev.MaxAttempts);
  out << ",C:" << to_string(Ev.CutoffAttempts);
  out << ",R:" << to_string(Ev.Rank);
  out << "}\n";
  return out;
}

void TestStuff()
{
  WCAEvent &e = WCAEvent::Get("333");
  cout << "Addr for 333: " << &e << "\n";
  WCAEvent &e2 = WCAEvent::Get("333");
  cout << "Addr for 333: " << &e2 << "\n";
}

}
