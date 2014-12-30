#pragma once
#include <windows.h>
#include <gdiplus.h>
#pragma comment (lib, "Gdiplus.lib")
#include <string>
#include <queue>
#include <concurrent_queue.h>
#include <memory>

#include "UIComponent.h"
#include "UIEvent.h"

using namespace std;

class Window
{
public:
  Window(void);
  virtual ~Window(void);
  void show();
  void hide();
  void paint();
  void setSize(long width, long height);
  void setTitle(wstring title);
  void setBackgroundColor(Gdiplus::Color color);
  void addUIComponent(std::shared_ptr<UIComponent> component);
  bool isOpen();
  bool pollEvents(UIEvent& ev);
private:
  std::vector<std::shared_ptr<UIComponent>> m_uiComponents;
  HWND m_handle;
  Gdiplus::Color m_bgColor;
  concurrency::concurrent_queue<UIEvent> m_eventQueue;
  bool m_isOpen;
  int m_width;
  int m_height;
  void propagate(UIEvent ev);
  void registerWindowClass();
  void createWindow();
  void processEvents();
  void handleEvent(UINT message, WPARAM wParam, LPARAM lParam);
  static LRESULT CALLBACK globalWindowsEvent(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};