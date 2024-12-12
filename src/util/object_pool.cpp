#include "util/object_pool.h"

void* ObjectPool::poll() {
  std::lock_guard lock(mutex);
  if (pool.empty()) {
    return nullptr;
  }

  void* object = pool.front();
  pool.pop();
  return object;
}

void ObjectPool::push(void* entity) {
  std::lock_guard lock(mutex);
  pool.push(entity);
}