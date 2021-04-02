#pragma once

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

#define real float
#pragma warning(disable:4305) // warning C4305: 'initializing': truncation from 'double' to 'float'
#pragma warning(disable:4244) // warning C4244: 'return': conversion from 'double' to 'float', possible loss of data

// Usings

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants

const real infinity = std::numeric_limits<real>::infinity();
const real pi = 3.1415926535897932385;

// Utility Functions

inline real degrees_to_radians(real degrees) {
  return degrees * pi / 180.0;
}

inline real random_real() {
  // Returns a random real in [0,1).
  return rand() / (RAND_MAX + 1.0);
}

inline real random_real(real min, real max) {
  // Returns a random real in [min,max).
  return min + (max - min) * random_real();
}

inline real clamp(real x, real min, real max) {
  if (x < min) return min;
  if (x > max) return max;
  return x;
}

// Common Headers

#include "ray.h"
#include "vec3.h"