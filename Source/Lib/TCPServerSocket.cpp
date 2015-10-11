#include "../Include/TCPServerSocket.h"

TCPServerSocket::TCPServerSocket() : TCPSocket() {}
TCPServerSocket::~TCPServerSocket() {}

void TCPServerSocket::poll()
{
  sockaddr_in acceptAddress = {0};
  int length = sizeof(acceptAddress);
  SOCKET socket = ::accept(m_handle, (sockaddr*) &acceptAddress, &length);
  if ((INVALID_SOCKET != socket) || (WSAEWOULDBLOCK != WSAGetLastError()))
  {
    std::shared_ptr<TCPSocket> newSocket = std::make_shared<TCPSocket>(socket);
    newSocket->remote = acceptAddress;
    newSocket->local = local;
    SocketEvent ev;
    ev.type = SocketEvent::Connection;
    ev.socket = newSocket;
    trigger(ev);
  }
}

void TCPServerSocket::listen()
{
  m_error = ::listen(m_handle, SOMAXCONN);
  checkAndEmitError();
}
