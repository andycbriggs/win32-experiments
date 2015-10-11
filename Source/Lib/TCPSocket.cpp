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
  close();
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
  sockaddr_in sockaddr = {0};
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_addr.s_addr = inet_addr(ipaddress);
  sockaddr.sin_port = htons(port);

  m_error = ::connect(m_handle, (SOCKADDR *) &sockaddr, sizeof(sockaddr));

  if(!checkAndEmitError())
  {
    SocketEvent ev;
    ev.type = SocketEvent::Connected;
    trigger(ev);
  }
}

void TCPSocket::close()
{
  if (m_handle) {
    ::shutdown(m_handle, 2);
    ::closesocket(m_handle);
    m_handle = 0;
  }
}

void TCPSocket::bind(const char* ipaddress, const unsigned short port)
{
  sockaddr_in sockaddr = {0};
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_addr.s_addr = inet_addr(ipaddress);
  sockaddr.sin_port = htons(port);

  m_error = ::bind(m_handle, (SOCKADDR *) &sockaddr, sizeof(sockaddr));
  checkAndEmitError();
}

void TCPSocket::setBlocking(const bool block)
{
  if (m_handle != INVALID_SOCKET) {
    u_long nonBlocking = block ? 0 : (u_long) 1;
    m_error = ::ioctlsocket(m_handle, FIONBIO, &nonBlocking);
  }
}

void TCPSocket::setNoDelay(const bool noDelay)
{
  if (m_handle != INVALID_SOCKET) {
    char noDelay = 1;
    ::setsockopt(m_handle, IPPROTO_TCP, TCP_NODELAY, (char*) &noDelay, sizeof(noDelay));
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
    char buffer[BUFFER_SIZE] = {0};

    if (readableSize > BUFFER_SIZE) readableSize = BUFFER_SIZE;

    m_error = ::recv(m_handle, buffer, readableSize, 0);

    if (!checkAndEmitError()) { // data was received
      SocketEvent ev;
      ev.type = SocketEvent::Error;
      ev.data = buffer;
      ev.dataLength = readableSize;
      ev.socket = shared_from_this();
      trigger(ev);
    }
  }
}
