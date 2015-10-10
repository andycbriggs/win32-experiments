#pragma once

#include <Windows.h>
#include <queue>
#include <mutex>

#include "ScopedLock.h"

template<typename DataType>
class LockingQueue {
private:
  LockingQueue() { };

  virtual ~LockingQueue() 
  {
    _queue.empty();
  };

  bool tryPop(DataType& target)
  {
    std::lock_guard(_lock);

    if (!_queue.empty())
    {
      target = _queue.front();
      _queue.pop();
      return true;
    }
    return false;
  };

  void push(DataType& target)
  {
    std::lock_guard(_lock);
    _queue.push(std::copy(target));
  };

public:
  std::mutex _lock;
  std::queue<DataType> _queue;

};