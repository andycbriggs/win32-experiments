
#include "../Include/UDPSocket.h"

UDPSocket::UDPSocket() : 
  fromlen(sizeof(sockaddr))
{
  m_handle = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  setBlocking(false);
  setBroadcast(true);
}

UDPSocket::UDPSocket(SOCKET& handle) : Socket(handle),
  fromlen(sizeof(sockaddr))
{
  setBlocking(false);
  setBroadcast(true);
}

UDPSocket::~UDPSocket()
{

}

void UDPSocket::bind(const char* ipaddress, const unsigned short port)
{
  local.sin_family = AF_INET;
  local.sin_addr.s_addr = inet_addr(ipaddress);
  local.sin_port = htons(port);

  m_error = ::bind(m_handle, (sockaddr *) &local, sizeof(sockaddr));

  checkAndEmitError();
  // should probably fire a listening event
}

void UDPSocket::sendto(const char* data, const int& length, const char* ipaddress, const unsigned short port)
{
  remote.sin_family = AF_INET;
  remote.sin_addr.s_addr = inet_addr(ipaddress);
  remote.sin_port = htons(port);

  m_error = ::sendto(m_handle, data, length, 0, (sockaddr*) &remote, sizeof(sockaddr));

  checkAndEmitError();
}

void UDPSocket::sendto(const std::string& message, const char* ipaddress, const unsigned short port)
{
  sendto(message.c_str(), message.size(), ipaddress, port);
}

void UDPSocket::setBroadcast(const bool broadcast)
{
  if (m_handle != INVALID_SOCKET) {
    char optval = broadcast ? 1 : 0;
    ::setsockopt(m_handle, IPPROTO_UDP, SO_BROADCAST, (char*) &broadcast, sizeof(char));
  }
}

void UDPSocket::poll()
{
  if (checkAndEmitError()) {
    return;
  }

  u_long readableSize = 0;
  m_error = ::ioctlsocket(m_handle, FIONREAD, &readableSize);

  if ((SOCKET_ERROR != m_error) && (readableSize > 0)) {
    if (readableSize > BUFFER_SIZE) readableSize = BUFFER_SIZE;

    clearBuffer();

    m_error = ::recvfrom(m_handle, buffer, readableSize, 0, (sockaddr*) &remote, &fromlen);

    if (!checkAndEmitError()) {
      SocketEvent ev;
      ev.type = SocketEvent::Data;
      ev.data = buffer;
      ev.dataLength = readableSize;
      ev.socket = std::dynamic_pointer_cast<Socket>(shared_from_this());
      trigger(ev); 
    }

  }

}