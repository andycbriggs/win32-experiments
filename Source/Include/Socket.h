#pragma once

#include <windows.h>
#pragma comment(lib, "Ws2_32.lib")

#include "EventEmitter.h"
#include "SocketEvent.h"
#include "NonCopyable.h"

class Socket : public EventEmitter<SocketEvent, SocketEvent::Type>, NonCopyable
{
public:
  Socket(void) {};
  Socket(SOCKET& handle) : m_handle(handle) {};

  virtual void poll() = 0;

  inline bool operator==(const Socket& target){ return (m_handle == target.m_handle); };
  inline bool operator!=(const Socket& target){ return !(m_handle == target.m_handle); };

protected:
  bool checkAndEmitError();

  SOCKET m_handle;
  int m_error;
};