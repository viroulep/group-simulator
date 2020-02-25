#ifndef LIBSIMU_HPP
#define LIBSIMU_HPP

#include <string>
#include <iostream>

int LoadCostsModel(const std::string &Filename);

void EmitConfig(std::ostream &out);

#endif
