#include "math/vectors.h"

#define DEGREES_TO_RADIANS 0.017453292519943295f
#define RADIANS_TO_DEGREES 57.29577951308232f

bool intersect(float fDst1, float fDst2, vec2f& P1, vec2f& P2, vec2f& B1, vec2f& B2, const int point) {
  if (fDst1 == fDst2 || (fDst1 * fDst2) >= 0.0f) return false;
  float hit = P1[point] + (P2[point] - P1[point]) * (-fDst1 / (fDst2 - fDst1));
  return hit > B1[point] && hit < B2[point];
}

bool rayIntersect(vec2f B1, vec2f B2, vec2f L1, vec2f L2) {
  if ((L2.x() < B1.x() && L1.x() < B1.x())
      || (L2.x() > B2.x() && L1.x() > B2.x())
      || (L2.y() < B1.y() && L1.y() < B1.y())
      || (L2.y() > B2.y() && L1.y() > B2.y())) {
    return false;
  }

  if (L1.x() > B1.x() && L1.x() < B2.x() &&
      L1.y() > B1.y() && L1.y() < B2.y()) {
    return true;
  }

  return intersect(L1.x() - B1.x(), L2.x() - B1.x(), L1, L2, B1, B2, 1)
         || intersect(L1.y() - B1.y(), L2.y() - B1.y(), L1, L2, B1, B2, 0)
         || intersect(L1.x() - B2.x(), L2.x() - B2.x(), L1, L2, B1, B2, 1)
         || intersect(L1.y() - B2.y(), L2.y() - B2.y(), L1, L2, B1, B2, 0);
}

float radians(float angle) {
  return angle * DEGREES_TO_RADIANS;
}

float degrees(float angle) {
  return angle * RADIANS_TO_DEGREES;
}

float lerp(float from, float to, float delta) {
  return from + (to - from) * delta;
}
