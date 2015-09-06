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

TCPSocket::TCPSocket()
{

  m_handle = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (m_handle != INVALID_SOCKET) {
    unsigned long blocking = 1;
    ::ioctlsocket(m_handle, FIONBIO, &blocking); // set non-blocking
    ::setsockopt(m_handle, IPPROTO_TCP, TCP_NODELAY, (char*) true, sizeof(int));
  }
}

TCPSocket::TCPSocket(SOCKET& handle) :
  m_handle(handle)
{
}

TCPSocket::~TCPSocket()
{
  ::closesocket(m_handle);
  eventHandlers.clear();
}

void TCPSocket::on(SocketEvent::Type eventType, std::function<void(SocketEvent)> handler)
{
  eventHandlers.insert(std::pair<SocketEvent::Type, std::function<void(SocketEvent)>>(eventType, handler));
}

void TCPSocket::trigger(SocketEvent ev)
{
  // fire user event handles
  if (eventHandlers.count(ev.type) > 0)
  {
    eventHandlers.at(ev.type)(ev);
  }
}

void TCPSocket::send(const std::string data)
{
  int error = ::send(m_handle, data.c_str(), sizeof(data.c_str()), 0);
  if (SOCKET_ERROR == error) {
    SocketEvent ev;
    ev.error = WSAGetLastError();
    ev.type = SocketEvent::Error;
    trigger(ev);
  }
}

void TCPSocket::connect(const std::string ipaddress, unsigned short port)
{
  sockaddr_in sockaddr;
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_addr.s_addr = inet_addr(ipaddress.c_str());
  sockaddr.sin_port = port;

  int error = ::connect(m_handle, (SOCKADDR *) &sockaddr, sizeof(sockaddr));

  if ((SOCKET_ERROR == error) && (WSAEWOULDBLOCK != WSAGetLastError())) {
    SocketEvent ev;
    ev.error = WSAGetLastError();
    ev.type = SocketEvent::Error;
    trigger(ev);
  }
  else
  {
    SocketEvent ev;
    ev.type = SocketEvent::Connected;
    trigger(ev);
  }
}

void TCPSocket::disconnect()
{
  int error = ::shutdown(m_handle, 2);
  if (SOCKET_ERROR == error) {
    SocketEvent ev;
    ev.error = WSAGetLastError();
    ev.type = SocketEvent::Error;
    trigger(ev);
  }
}

void TCPSocket::bind(const std::string ipaddress, unsigned short port)
{
  sockaddr_in sockaddr;
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_addr.s_addr = inet_addr(ipaddress.c_str());
  sockaddr.sin_port = port;
  int error = ::bind(m_handle, (SOCKADDR *) &sockaddr, sizeof(sockaddr));
  if (SOCKET_ERROR == error) {
    SocketEvent ev;
    ev.error = WSAGetLastError();
    ev.type = SocketEvent::Error;
    trigger(ev);
  }
}

void TCPSocket::listen()
{
  int error = ::listen(m_handle, SOMAXCONN);
  if (SOCKET_ERROR == error) {
    SocketEvent ev;
    ev.error = WSAGetLastError();
    ev.type = SocketEvent::Error;
    trigger(ev);
  }
}

void TCPSocket::poll()
{

  char buffer[512];
  int error = ::recv(m_handle, buffer, sizeof(buffer), 0);
  if ((SOCKET_ERROR == error) && (WSAEWOULDBLOCK != WSAGetLastError()))
  {
    SocketEvent ev;
    //ev.type = SocketEvent::Error;
    //ev.error = WSAGetLastError();
    //trigger(ev);
  }
  else if (error > 0) // data was received
  {
    SocketEvent ev;
    ev.type = SocketEvent::Data;
    ev.data = std::string(buffer);
    trigger(ev);
  }

  SOCKET socket = ::accept(m_handle, 0, 0);
  if (INVALID_SOCKET != socket)
  {
    std::shared_ptr<TCPSocket> newsocket = std::make_shared<TCPSocket>(socket);
    SocketEvent ev;
    ev.type = SocketEvent::Connection;
    ev.socket = newsocket;
    trigger(ev);
  }

}