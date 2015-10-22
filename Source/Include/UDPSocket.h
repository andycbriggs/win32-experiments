#pragma once

#include <string>
#include <vector>

#include "Socket.h"

class UDPSocket : public Socket, public std::enable_shared_from_this<UDPSocket>
{
public:
  UDPSocket();
  UDPSocket(SOCKET& handle);
  virtual ~UDPSocket();

  void bind(const char* ipaddress, const unsigned short port);
  virtual void sendto(const char* data, const int& length, const char* ipaddress, const unsigned short port);
  virtual void sendto(const std::string& message, const char* ipaddress, const unsigned short port);

  void setBroadcast(const bool broadcast);

  virtual void poll() override;

private:
  
  int fromlen;
};