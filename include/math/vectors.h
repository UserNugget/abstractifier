#ifndef ABSTRACTIFIER_VECTORS_H
#define ABSTRACTIFIER_VECTORS_H

#define _USE_MATH_DEFINES
#include <cmath>
#include <array>
#include "util/lib.h"

template<typename T, size_t S>
using vec = std::array<T, S>;

typedef vec<float, 2> vec2f;
typedef vec<float, 6> vec6f;
typedef vec<int, 2> vec2i;

float length(vec2f first);
float angle(vec2f first, vec2f second);

#endif //ABSTRACTIFIER_VECTORS_H
