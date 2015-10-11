#pragma once

#include "WindowsCommon.h"

#include <functional>
#include <map>

#include "UIEvent.h"

class UIComponent
{
public:
  UIComponent(void);
  virtual ~UIComponent(void);
  virtual void paint(Gdiplus::Graphics& graphics) =0;
  virtual void trigger(UIEvent ev);
  virtual void on(UIEvent::Type eventType, std::function<void(UIEvent)> handler);
  virtual void setPosition(float x, float y);
  virtual void setSize(float x, float y);
  bool hitTest(int x, int y);
protected:
  bool m_mouseOver;
  float m_x;
  float m_y;
  float m_width;
  float m_height;
private:
  std::map<UIEvent::Type, std::function<void(UIEvent)>> uiEventHandlers;
};

