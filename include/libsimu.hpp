#ifndef LIBSIMU_HPP
#define LIBSIMU_HPP

#include <string>
#include <iostream>

namespace libsimu {

using Time = uint64_t;

std::error_code LoadModelCosts(const std::string &Filename);
std::error_code LoadScramblingCost(const std::string &Filename);
std::error_code LoadConfig(const std::string &Filename);

void TestStuff();

void EmitConfig(std::ostream &out);

//ConfigScramblers(n, double miscramblerate)
//ConfigJudges(n, double extrarate)

std::error_code SimuGroup(const std::string &EventId, unsigned int N);
//SimuGroup(event, vector<avg> list)

}
#endif
