#include "math/vectors.h"

float angle(vec2f first, vec2f second) {
  return std::atan2(first[0] - second[0], first[1] - second[1]) + (float) M_PI;
}
