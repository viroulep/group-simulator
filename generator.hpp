#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include "types.hpp"
#include <string>
#include <set>

class CubeLoader {
  public:
  static CubeSet loadFile(const std::string &filename);
};

#endif
