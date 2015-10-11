#pragma once

#include <windows.h>

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

  virtual void connect(const char* ipaddress, const unsigned short port);
  virtual void bind(const char* ipaddress, const unsigned short port);
  virtual void close();

  virtual void setBlocking(const bool block);
  virtual void setNoDelay(const bool noDelay);

  virtual void poll() override;

};