#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include "types.hpp"
#include <string>
#include <set>
#include <random>
#include <cmath>
#include <iostream>

class CubeLoader {
  public:
  static CubeSet loadFile(const std::string &filename);
  template<typename D>
  static CubeSet random(size_t number, D &distribution)
  {
    CubeSet ret;
    std::default_random_engine generator(std::random_device{}());
    for (size_t i = 0; i < number; ++i) {
      double t = -1;
      while (t < 0)
        t = distribution(generator);
      ret.insert(new Cube(lround(t)));
    }
    return ret;
  }
};

#endif
