#include "../Include/TCPSocket.h"

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
  disconnect();
  ::closesocket(m_handle);
  m_handle = 0;
}

void TCPSocket::send(const std::string data)
{
  m_error = ::send(m_handle, data.c_str(), sizeof(data.c_str()), 0);
  checkAndEmitError();
}

void TCPSocket::connect(const std::string ipaddress, unsigned short port)
{
  sockaddr_in sockaddr = {0};
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_addr.s_addr = inet_addr(ipaddress.c_str());
  sockaddr.sin_port = htons(port);

  m_error = ::connect(m_handle, (SOCKADDR *) &sockaddr, sizeof(sockaddr));

  if(!checkAndEmitError())
  {
    SocketEvent ev;
    ev.type = SocketEvent::Connected;
    trigger(ev);
  }
  else
  {
    SocketEvent ev;
    ev.type = SocketEvent::Error;
    ev.error = 999;
    trigger(ev);
  }
}

void TCPSocket::disconnect()
{
  m_error = ::shutdown(m_handle, 2);
  checkAndEmitError();
}

void TCPSocket::bind(const std::string ipaddress, unsigned short port)
{
  sockaddr_in sockaddr = {0};
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_addr.s_addr = inet_addr(ipaddress.c_str());
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

  char buffer[512] = {0};

  int recvError = ::recv(m_handle, buffer, sizeof(buffer), 0);
  if ((SOCKET_ERROR != recvError) || (WSAEWOULDBLOCK != WSAGetLastError())) // data was received
  {
    SocketEvent ev;
    ev.type = SocketEvent::Data;
    ev.data = std::string(buffer);
    trigger(ev);
  }

}
