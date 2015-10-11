#pragma once

#include <memory>
#include "NonCopyable.h"

class TCPSocket;

struct SocketEvent : public NonCopyable {
  enum Type {
    Error,
    Data,
    Connection,
    Connected,
    Close
  };
  Type type;
  int error;
  char* data;
  int dataLength;
  std::shared_ptr<TCPSocket> socket;
};
