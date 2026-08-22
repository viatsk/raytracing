#ifndef COMMON_H
#define COMMON_H

#include <random>

// Number + Trig Utilities
inline double random_double() {
    std::random_device rd;
    std::mt19937 generate(rd());

    std::uniform_real_distribution<double> distribution(-1.0, 1.0);
    return distribution(generate);
}

const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}


#endif
