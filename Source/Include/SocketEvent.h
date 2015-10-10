#pragma once

#include <memory>
#include <string>

#include "NonCopyable.h"

class TCPSocket;

struct SocketEvent {
  enum Type {
    Error,
    Data,
    Connection,
    Connected
  };
  Type type;
  int error;
  char* data;
  int dataLength;
  std::shared_ptr<TCPSocket> socket;
};
