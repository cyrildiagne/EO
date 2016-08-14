#ifndef eo_utils_h
#define eo_utils_h

#include <random>

namespace eo {
namespace utils {

template <class T> T random(T min, T max) {
  // prepare random distribution
  std::random_device rd;
  std::mt19937 d(rd());
  std::uniform_real_distribution<> r(0, 1);
  // return mapped values
  return min + r(d) * (max - min);
}

template <class T> T random(T max) { return random(static_cast<T>(0), max); }

} // namespace utils
} // namespace eo

#endif /* end of include guard: eo_utils_h */
