#pragma once

#include <windows.h>

#include <string>
#include <vector>

#include "Socket.h"

class TCPSocket : public Socket
{
public:
  TCPSocket();
  TCPSocket(SOCKET& handle);
  virtual ~TCPSocket();

  virtual void send(const char* data, const int& length);
  virtual void send(const std::string message);
  virtual void connect(const std::string ipaddress, const unsigned short port);
  virtual void disconnect();
  virtual void bind(const std::string ipaddress, const unsigned short port);

  virtual void setBlocking(const bool block);
  virtual void setNoDelay(const bool noDelay);

  virtual void poll() override;

};