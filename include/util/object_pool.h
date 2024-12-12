#ifndef ABSTRACTIFIER_OBJECT_POOL_H
#define ABSTRACTIFIER_OBJECT_POOL_H

#include <queue>
#include <mutex>

class ObjectPool {
private:
  std::queue<void*> pool = std::queue<void*>();
  std::mutex mutex;

public:
  void push(void* entity);
  void* poll();
};

#endif
