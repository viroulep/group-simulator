#ifndef LIBSIMU_HPP
#define LIBSIMU_HPP

#include <string>
#include <iostream>

using Time = uint64_t;

int LoadModelCosts(const std::string &Filename);
int LoadScramblingCost(const std::string &Filename);

void EmitConfig(std::ostream &out);

#endif
