#pragma once

#include <memory>
#include <string>

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
  std::string data;
  std::shared_ptr<TCPSocket> socket;
};
