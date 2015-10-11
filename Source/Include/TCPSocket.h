#pragma once

#include <string>
#include <vector>

#include "Socket.h"

class TCPSocket : public Socket, public std::enable_shared_from_this<TCPSocket>
{
public:
  TCPSocket();
  TCPSocket(SOCKET& handle);
  virtual ~TCPSocket();

  virtual void send(const char* data, const int& length);
  virtual void send(const std::string& message);

  void connect(const char* ipaddress, const unsigned short port);
  void bind(const char* ipaddress, const unsigned short port);
  void close();

  void setBlocking(const bool block);
  void setNoDelay(const bool noDelay);

  void getPeerAddress();

  virtual void poll() override;

};