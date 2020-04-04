#include "libsimu.hpp"
#include "Config.hpp"
#include "Debug.hpp"
#include "Cube.hpp"
#include "GroupSimulator.hpp"

using namespace std;

namespace libsimu {

void EmitConfig() {
  cout << Model::get();
  cout << Scrambling::get();
  cout << Setup::get();
}

ostream &operator<<(ostream &os, const set<std::unique_ptr<Cube>> &C)
{
  os << "[";
  for (const unique_ptr<Cube> &C : C) {
      os << *C << ", ";
  }
  os << "]";
  return os;
}

ostream &operator<<(ostream &out, const Model &MC)
{
  out << "Model {\n";
  out << "  InitGroup: " << MC.InitGroup << "\n";
  out << "  RunIn: " << MC.RunIn << "\n";
  out << "  CompetitorReady: " << MC.CompetitorReady << "\n";
  out << "  CompetitorCleanup: " << MC.CompetitorCleanup << "\n";
  out << "  RunOut: " << MC.RunOut << "\n";
  out << "  ShutdownGroup: " << MC.ShutdownGroup << "\n";
  out << "}\n";
  return out;
}

ostream &operator<<(ostream &out, const Scrambling &SC)
{
  out << "Scrambling {\n";
  for (auto &entry : SC) {
    out << string("  ") << entry.first << ": " << entry.second << "\n";
  }
  out << "}\n";
  return out;
}

ostream &operator<<(ostream &out, const Setup &C)
{
  // zext the uint8_t so that it's not considered as a (non-printable) char.
  out << "Setup {\n";
  out << "  CubesPerRunner: " << to_string(C.MaxCubes) << "\n";
  out << "  ExtraRate: " << to_string(C.ExtraRate) << "\n";
  out << "  MiscrambleRate: " << to_string(C.MiscrambleRate) << "\n";
  out << "  Judges: " << to_string(C.Judges) << "\n";
  out << "  Runners: " << to_string(C.Runners) << "\n";
  out << "  Scramblers: " << to_string(C.Scramblers) << "\n";
  out << "  TimeLimit: " << to_string(C.TimeLimit) << "\n";
  out << "  Cutoff: " << to_string(C.Cutoff) << "\n";
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
  out << "Simulator {\n";
  out << "  walltime: " << GS.Walltime << "\n";
  out << "  activeCubes: " << GS.ActiveCubes << "\n";
  out << "  idle scramblers: " << GS.ScramblersAvailable << "\n";
  out << "  pendingScramble: " << GS.PendingScramble << "\n";
  out << "  pendingRunIn: " << GS.ScrambledCubes << "\n";
  out << "  pendingRunOut: " << GS.SolvedCubes << "\n";
  out << "  judges: " << GS.Judges << "\n";
  out << "  remaining events: " << GS.Events << "\n";
  GS.EmitToStream(out);
  out << "}\n";
  return out;
}

ostream &RunnerSystemSimulator::EmitToStream(ostream &out) const
{
  out << "  RunnerSystemSimulator {\n";
  out << "  }\n";
  return out;
}

ostream &JudgesRunSimulator::EmitToStream(ostream &out) const
{
  out << "  JudgesRunSimulator {\n";
  out << "  judgesIdle_: " << judgesIdle_ << "\n";
  out << "  }\n";
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

int TestStuff(int a)
{
  WCAEvent &e = WCAEvent::Get(WCAEvent::E_333);
  cout << "Addr for 333: " << &e << "\n";
  WCAEvent &e2 = WCAEvent::Get(WCAEvent::E_333);
  cout << "Addr for 333: " << &e2 << "\n";
  return 2*a;
}

}
