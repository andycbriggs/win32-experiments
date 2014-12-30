
#pragma once

#include "UIComponent.h"

class Notification :
  public UIComponent
{
public:

  const static UINT WM_TRAYICONCLICKED = WM_USER + 1776;

  Notification(void);
  virtual ~Notification(void);
  void handleWMEvent(UIEvent ev);
  void setWindow(HWND hWnd);
  void show();
  void hide();
  void loadIcon(std::wstring filename);
  void setIcon(HICON icon);
private:
  NOTIFYICONDATA m_iconData;
};