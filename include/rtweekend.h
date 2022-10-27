
#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <limits>
#include <memory>

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degress_to_radians(double degrees) {
    return degrees * pi / 180;
}

// Common headers

#include "ray.h"
#include "vec3.h"

#endif // RTWEEKEND_H