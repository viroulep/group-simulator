#ifndef ACTORS_HPP
#define ACTORS_HPP

#include <llvm/ObjectYAML/ObjectYAML.h>
#include <cstdint>
#include <iostream>
#include "libsimu.hpp"

namespace libsimu {

struct Judge {
  Time busyUntil;
  Time idleTime;

  // Between 0 and 100, 0 means no extra can occur
  static uint8_t ExtraRate;
  bool operator<(const Judge &r) const;
  friend std::ostream &operator<<(std::ostream &out, const Judge &J);
};

struct Runner {
  static uint8_t MaxCubes;
  friend std::ostream &operator<<(std::ostream &out, const Runner &R);
};

struct Scrambler {
  // Between 0 and 100, 0 means no miscramble
  static uint8_t MiscrambleRate;
  friend std::ostream &operator<<(std::ostream &out, const Scrambler &S);
};

// Proxy class to (re)-fill J/R/S static data.
// May contain more configuration later.
struct Config {
  static Config &get();
  static const Time MaxTimeLimit = 600;
  uint8_t Judges = 10;
  uint8_t Runners = 2;
  uint8_t Scramblers = 3;
  Time TimeLimit = MaxTimeLimit;
  Time Cutoff = 600;
  friend std::ostream &operator<<(std::ostream &out, const Config &C);
private:
  Config() = default;
};

}

template <>
struct llvm::yaml::MappingTraits<libsimu::Config> {
  static void mapping(IO &io, libsimu::Config &C) {
    io.mapOptional("extra_rate", libsimu::Judge::ExtraRate);
    io.mapOptional("cubes_per_runner", libsimu::Runner::MaxCubes);
    io.mapOptional("miscramble_rate", libsimu::Scrambler::MiscrambleRate);
    io.mapOptional("judges", C.Judges);
    io.mapOptional("runners", C.Runners);
    io.mapOptional("scramblers", C.Scramblers);
    io.mapOptional("time_limit", C.TimeLimit);
    io.mapOptional("cutoff", C.Cutoff);
  }
};
#endif
