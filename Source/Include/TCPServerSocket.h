#pragma once

#include <windows.h>

#include "TCPSocket.h"

class TCPServerSocket : public TCPSocket
{
public:

  TCPServerSocket();
  virtual ~TCPServerSocket();

  virtual void poll() override;
  virtual void listen();
};