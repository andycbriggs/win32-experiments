#pragma once

#include <windows.h>
#pragma comment(lib, "Ws2_32.lib")

#include <memory>
#include <algorithm>

#include "EventEmitter.h"
#include "SocketEvent.h"
#include "NonCopyable.h"

class Socket : public EventEmitter<SocketEvent, SocketEvent::Type>, NonCopyable
{
public:
  Socket() { clearBuffer(); };
  Socket(SOCKET& handle) : m_handle(handle) {};

  virtual void poll() = 0;

  inline bool operator==(const Socket& target){ return (m_handle == target.m_handle); };
  inline bool operator!=(const Socket& target){ return !(*this == target); };

protected:
  bool checkAndEmitError();
  inline void clearBuffer() { std::fill(&buffer[0], &buffer[BUFFER_SIZE+1], '\0'); }

  static const unsigned int BUFFER_SIZE = 64 * 1024;
  char buffer[BUFFER_SIZE+1]; // use +1 here so that we can always have a null terminated array

  SOCKET m_handle;
  int m_error;
};