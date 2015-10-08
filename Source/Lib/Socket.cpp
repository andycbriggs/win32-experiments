#include "../Include/Socket.h"

bool Socket::checkAndEmitError()
{
  if ((SOCKET_ERROR == m_error) && (WSAEWOULDBLOCK != WSAGetLastError()))
  {
    SocketEvent ev;
    ev.type = SocketEvent::Error;
    ev.error = WSAGetLastError();
    trigger(ev);
    m_error = 0;
    return true;
  }
  m_error = 0;
  return false;
}