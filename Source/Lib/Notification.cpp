
#include "../Include/Notification.h"

Notification::Notification(void)
{
  m_iconData.uFlags =  NIF_ICON | NIF_MESSAGE;
  m_iconData.uCallbackMessage = WM_TRAYICONCLICKED;
  m_iconData.uID = 10;
  std::function<void(const UIEvent)> wmEventHandler = std::bind(&Notification::handleWMEvent, this, std::placeholders::_1);
  on(UIEvent::WM, wmEventHandler);
}

Notification::~Notification(void)
{
  ::Shell_NotifyIcon(NIM_DELETE, &m_iconData);
}

void Notification::handleWMEvent(UIEvent ev)
{
  if (ev.type == UIEvent::WM) {
    switch (ev.wmMessage)
    {
    case WM_TRAYICONCLICKED:
      // tray icon was clicked
      break;
    }
  }
}

void Notification::setWindow(HWND hWnd)
{
  m_iconData.hWnd = hWnd;
}

void Notification::show()
{
  ::Shell_NotifyIcon(NIM_ADD, &m_iconData);
}

void Notification::hide()
{
  ::Shell_NotifyIcon(NIM_DELETE, &m_iconData);
}

void Notification::loadIcon(std::wstring filename)
{
  m_iconData.hIcon = (HICON) ::LoadImage(
    NULL,
    filename.c_str(),
    IMAGE_ICON,
    0,
    0,
    LR_LOADFROMFILE | LR_DEFAULTSIZE
  );
  ::Shell_NotifyIcon(NIM_MODIFY, &m_iconData);
}

void Notification::setIcon(HICON icon)
{
  m_iconData.hIcon = icon;
  ::Shell_NotifyIcon(NIM_MODIFY, &m_iconData);
}