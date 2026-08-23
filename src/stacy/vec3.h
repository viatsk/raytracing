#ifndef VEC3_H
#define VEC3_H

#include "common.h"

#include <cmath> 
#include <iostream>

class vec3 {
 public:
  double e[3];

  // Constructors
  vec3() : e{0, 0, 0} {}
  vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

  // Access
  double x() const { return e[0]; }
  double y() const { return e[1]; }
  double z() const { return e[2]; }

  // Operators
  vec3 operator-() const { return vec3{-e[0], -e[1], -e[2]}; }
  double operator[](int i) const { return e[i]; }
  double& operator[](int i) { return e[i]; }

  vec3& operator+=(const vec3& v) {
      e[0] += v.e[0];
      e[1] += v.e[1];
      e[2] += v.e[2];
      return *this;
  }

  vec3& operator*=(double t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
  }

  vec3& operator/=(double t) {
    return *this *= 1/t;
  }

  // Convenience methods
  double length() const {
    return std::sqrt(length_squared());
  }

  double length_squared() const {
    return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
  }

  bool near_zero() const {
    // Return true if the vector is close to zero in all dimensions.
    auto s = 1e-8;
    return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
  }
};

using point3 = vec3;

// Vector Utility Functions

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline bool operator==(const vec3& u, const vec3& v) {
    return ((u.e[0] == v.e[0]) && (u.e[1] == v.e[1]) && (u.e[2] == v.e[2]));
}


inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3& v) {
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline vec3 operator*(const vec3& v, double t) {
    return t * v;
}

inline vec3 operator/(const vec3& v, double t) {
    return (1/t) * v;
}

inline double dot(const vec3& u, const vec3& v) {
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(const vec3& v) {
    return v / v.length();
}

inline static vec3 random_vec3() {
  return vec3(random_double(), random_double(), random_double());
}

// TODO this sucks and I hate it.
inline vec3 random_unit_vector() {
    while (true) {
        vec3 p = random_vec3();
        auto lensq = p.length_squared();
        if (1e-5 < lensq && lensq <= 1)
            return p / sqrt(lensq);
    }
}

// Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
inline vec3 sample_unit_square() {
  return vec3(random_double(0.0, 1.0) - 0.5, random_double(0.0, 1.0) - 0.5, 0);
}

// TODO: Confirm this works.
// inline vec3 random_unit_vector_civilized() {
//   vec3 unit_vector;
//   for (int j = 0; j < 3; j++) {
//     unit_vector.e[j] = random_double();
//   }
//   double normal = unit_vector.length();
//   for (int j = 0; j < 3; j++) {
//     unit_vector.e[j] = unit_vector.e[j] / normal;
//   }
//   return unit_vector;
// }

// inline vec3 random_on_hemisphere(const vec3& normal) {
//   vec3 random_unit_vec = random_unit_vector();
//   return (dot(random_unit_vec, normal) > 0.0) ? random_unit_vec : -random_unit_vec;
// }


#endif
