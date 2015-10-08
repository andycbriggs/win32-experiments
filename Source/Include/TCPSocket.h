#pragma once

#include <windows.h>

#include <string>

#include "Socket.h"

class TCPSocket : public Socket
{
public:
  TCPSocket();
  TCPSocket(SOCKET& handle);
  virtual ~TCPSocket();

  virtual void send(const std::string data);
  virtual void connect(const std::string ipaddress, unsigned short port);
  virtual void disconnect();
  virtual void bind(const std::string ipaddress, unsigned short port);
  virtual void listen();

  virtual void poll() override;

};