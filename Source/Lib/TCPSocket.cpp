#include "../Include/TCPSocket.h"

#include <iostream>

TCPSocket::TCPSocket() : Socket()
{
  m_handle = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  setNoDelay(true);
  setBlocking(false);
}

TCPSocket::TCPSocket(SOCKET& handle) : Socket(handle)
{
  setNoDelay(true);
  setBlocking(false);
}

TCPSocket::~TCPSocket()
{
  // socket must be closed by user code
}

void TCPSocket::send(const char* data, const int& length)
{
  m_error = ::send(m_handle, data, length, 0);
  checkAndEmitError();
}

void TCPSocket::send(const std::string& message)
{
  send(message.c_str(), message.size());
}

void TCPSocket::connect(const char* ipaddress, const unsigned short port)
{
  remote.sin_family = AF_INET;
  remote.sin_addr.s_addr = inet_addr(ipaddress);
  remote.sin_port = htons(port);

  m_error = ::connect(m_handle, (sockaddr *) &remote, sizeof(sockaddr));

  if(!checkAndEmitError())
  {
    SocketEvent ev;
    ev.type = SocketEvent::Connected;
    ev.socket = shared_from_this();
    trigger(ev);
  }
}

void TCPSocket::bind(const char* ipaddress, const unsigned short port)
{
  local.sin_family = AF_INET;
  local.sin_addr.s_addr = inet_addr(ipaddress);
  local.sin_port = htons(port);

  m_error = ::bind(m_handle, (SOCKADDR*) &local, sizeof(sockaddr));
  checkAndEmitError();
}

void TCPSocket::setNoDelay(const bool noDelay)
{
  if (m_handle != INVALID_SOCKET) {
    char optval = noDelay ? 1 : 0;
    ::setsockopt(m_handle, IPPROTO_TCP, TCP_NODELAY, (char*) &optval, sizeof(char));
  }
}

void TCPSocket::poll()
{
  if (checkAndEmitError()) {
    return;
  };

  u_long readableSize = 0;
  m_error = ::ioctlsocket(m_handle, FIONREAD, &readableSize);

  if ((SOCKET_ERROR != m_error) && (readableSize > 0)) {
    if (readableSize > BUFFER_SIZE) readableSize = BUFFER_SIZE;

    clearBuffer();

    m_error = ::recv(m_handle, buffer, readableSize, 0);

    if (!checkAndEmitError()) { // data was received
      SocketEvent ev;
      ev.type = SocketEvent::Data;
      ev.data = buffer;
      ev.dataLength = readableSize;
      ev.socket = std::dynamic_pointer_cast<Socket>(shared_from_this());
      trigger(ev);
    }
  } else if (0 == ::recv(m_handle, buffer, BUFFER_SIZE, MSG_PEEK)) {
    // socket closed
    close();
    SocketEvent ev;
    ev.type = SocketEvent::Close;
    ev.socket = std::dynamic_pointer_cast<Socket>(shared_from_this());
    trigger(ev);
  }
}
