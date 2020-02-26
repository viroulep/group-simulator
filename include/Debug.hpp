#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <iostream>

namespace libsimu {

// Generic stream operator for collections (but not strings!)
template<typename T,
  std::enable_if_t<std::is_default_constructible_v<typename T::iterator>, int> = 0,
  std::enable_if_t<!std::is_same_v<T, std::string>, int> = 0
  >
std::ostream &operator<<(std::ostream &os , const T &s)
{
  os << "[";
  for (typename T::value_type c : s) {
    if constexpr (std::is_pointer_v<typename T::value_type>) {
      os << *c << ", ";
    } else {
      os << c << ", ";
    }
  }
  os << "]";
  return os;
}

}

#endif
