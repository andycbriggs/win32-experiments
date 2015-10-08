#include "../Include/TCPSocket.h"

struct WinsockInit
{
  WinsockInit()
  {
    WSADATA init;
    WSAStartup(MAKEWORD(2, 2), &init);
  }

  ~WinsockInit()
  {
    WSACleanup();
  }
};

namespace {
  WinsockInit winsockInit;
}

TCPSocket::TCPSocket() : Socket()
{
  m_handle = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (m_handle != INVALID_SOCKET) {
    unsigned long blocking = 1;
    ::ioctlsocket(m_handle, FIONBIO, &blocking); // set non-blocking
    //::setsockopt(m_handle, IPPROTO_TCP, TCP_NODELAY, (char*) true, sizeof(int));
  }
}

TCPSocket::TCPSocket(SOCKET& handle) : Socket(handle)
{
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
  sockaddr_in sockaddr;
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
}

void TCPSocket::disconnect()
{
  m_error = ::shutdown(m_handle, 2);
  checkAndEmitError();
}

void TCPSocket::bind(const std::string ipaddress, unsigned short port)
{
  sockaddr_in sockaddr;
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_addr.s_addr = inet_addr(ipaddress.c_str());
  sockaddr.sin_port = htons(port);
  m_error = ::bind(m_handle, (SOCKADDR *) &sockaddr, sizeof(sockaddr));
  checkAndEmitError();
}

void TCPSocket::listen()
{
  m_error = ::listen(m_handle, SOMAXCONN);
  checkAndEmitError();
}

void TCPSocket::poll()
{
  char buffer[512] = {0};

  int recvError = ::recv(m_handle, buffer, sizeof(buffer), 0);
  if ((SOCKET_ERROR == recvError) && (WSAEWOULDBLOCK != WSAGetLastError())) // data was received
  {
    // not connected
  }
  else if (SOCKET_ERROR != recvError) // data was received
  {
    SocketEvent ev;
    ev.type = SocketEvent::Data;
    ev.data = std::string(buffer);
    trigger(ev);
  }

  SOCKET socket = ::accept(m_handle, 0, 0);
  if (INVALID_SOCKET != socket)
  {
    std::shared_ptr<TCPSocket> newSocket = std::make_shared<TCPSocket>(socket);
    SocketEvent ev;
    ev.type = SocketEvent::Connection;
    ev.socket = newSocket;
    trigger(ev);
  }

}
