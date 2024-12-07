#include "util/lib.h"

uint64_t timeMillis() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

int64_t max(int64_t a, int64_t b) {
  return a > b ? a : b;
}