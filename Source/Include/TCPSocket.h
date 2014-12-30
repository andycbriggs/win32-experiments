#pragma once

#include <windows.h>
#pragma comment(lib, "Ws2_32.lib")

#include <string>
#include <functional>
#include <map>

#include "SocketEvent.h"

class TCPSocket
{
public:

  TCPSocket();
  TCPSocket(SOCKET& handle);
  virtual ~TCPSocket();

  virtual void on(SocketEvent::Type eventType, std::function<void(SocketEvent)> handler);
  virtual void trigger(SocketEvent ev);

  virtual void send(const std::string data);
  virtual void connect(const std::string ipaddress, unsigned short port);
  virtual void disconnect();
  virtual void bind(const std::string ipaddress, unsigned short port);
  virtual void listen();

  virtual void poll();
private:
  std::map<SocketEvent::Type, std::function<void(SocketEvent)>> eventHandlers;
  SOCKET m_handle;
};