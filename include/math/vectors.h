#ifndef ABSTRACTIFIER_VECTORS_H
#define ABSTRACTIFIER_VECTORS_H

#define _USE_MATH_DEFINES
#include <cmath>
#include <array>
#include "util/lib.h"

template<typename T>
class Vec : public std::array<T, 2> {
public:
  typedef Vec<T> type;
  typedef type& value;

  const T& x() const noexcept { return this->operator[](0); }
  const T& y() const noexcept { return this->operator[](1); }

  T& x() noexcept { return this->operator[](0); }
  T& y() noexcept { return this->operator[](1); }

  T angle(type second) {
    return std::atan2(x() - second.x(), y() - second.y());
  }

#define VEC_OPERATOR(operation) \
  constexpr type operator operation(type value) { \
    return type { x() operation value.x(), y() operation value.y() }; \
  } \
  constexpr type operator operation(T value) { \
    return type { x() operation value, y() operation value }; \
  }

  VEC_OPERATOR(+)
  VEC_OPERATOR(-)
  VEC_OPERATOR(/)
  VEC_OPERATOR(*)

  void operator=(type value) {
    this->operator[](0) = value[0];
    this->operator[](1) = value[1];
  }
};

typedef Vec<float> vec2f;
typedef Vec<int> vec2i;

bool rayIntersect(vec2f B1, vec2f B2, vec2f L1, vec2f L2);

float radians(float angle);
float degrees(float angle);

float lerp(float from, float to, float delta);

#endif
