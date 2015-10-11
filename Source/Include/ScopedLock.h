#pragma once

#include "WindowsCommon.h"

class ScopedLock {
  ScopedLock(CRITICAL_SECTION* criticalSection)
  {
    if (criticalSection)
    {
      _criticalSection = criticalSection;
      ::EnterCriticalSection(_criticalSection);
    }
  };

  ~ScopedLock()
  {
    if (_criticalSection)
    {
      ::LeaveCriticalSection(_criticalSection);
    }
  }

  CRITICAL_SECTION* _criticalSection;
};