#include "generator.hpp"
#include <yaml-cpp/yaml.h>

using namespace std;

CubeSet CubeLoader::loadFile(const string &filename)
{
  CubeSet ret;
  YAML::Node config = YAML::LoadFile(filename);
  auto cubes = config["cubes"].as<list<Time>>();
  for (Time t : cubes) {
    ret.insert(new Cube(t));
  }
  return ret;
}
