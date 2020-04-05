#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "libsimu.hpp"

#include <map>

namespace libsimu {

// FIXME: factorize the map load/serialization?

// Setup configuration.
// May contain more configuration later.
struct Setup {
  static Setup &get();
  static const Time MaxTimeLimit = 600;
  uint8_t Judges = 10;
  uint8_t Runners = 2;
  uint8_t Scramblers = 3;
  // Between 0 and 100, 0 means no extra can occur
  uint8_t ExtraRate = 0;
  // The maximum of cubes a runner can take
  uint8_t MaxCubes = 3;
  // Between 0 and 100, 0 means no miscramble
  uint8_t MiscrambleRate = 0;
  // FIXME: move this to simuparams
  Time TimeLimit = MaxTimeLimit;
  Time Cutoff = 600;
  ErrCodeTy loadMap(PropertiesMap const &Props);
  PropertiesMap asMap() const;
  friend std::ostream &operator<<(std::ostream &out, const Setup &C);
private:
  Setup() = default;
};

struct Model {
  Time InitGroup = 120;
  Time RunIn = 25;
  Time CompetitorReady = 30;
  Time CompetitorCleanup = 15;
  Time RunOut = 20;
  Time ShutdownGroup = 0;
  // Or CubesPerJudge if no runner
  unsigned int CubesPerRunner = 1;

  ErrCodeTy loadMap(PropertiesMap const &Props);
  PropertiesMap asMap() const;
  friend std::ostream &operator<<(std::ostream &out, const Model &CM);

  static Model &get();
private:
  Model() = default;
};

struct Scrambling : public std::map<std::string, Time> {
  static Scrambling &get();
  ErrCodeTy loadMap(PropertiesMap const &Props);
  friend std::ostream &operator<<(std::ostream &out, const Scrambling &CM);
private:
  Scrambling();
};

}

#endif
