#pragma once

#include <Windows.h>
#include <queue>

#include "ScopedLock.h"

template<typename DataType>
class LockingQueue {
private:
  LockingQueue() 
  {
    ::InitializeCriticalSection(&lock);
  };

  virtual ~LockingQueue() 
  {
    ::DeleteCriticalSection(&lock);
  };

  bool tryPop(DataType& target)
  {
    ScopedLock(&lock);

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
    ScopedLock(&lock);

    _queue.push(std::copy(target));
  };

public:
  CRITICAL_SECTION lock;
  std::queue<DataType> _queue;

};