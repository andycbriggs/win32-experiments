#include "UIComponent.h"

UIComponent::UIComponent(void) :
  m_x(0),
  m_y(0),
  m_mouseOver(false)
{
}

UIComponent::~UIComponent(void)
{
}

bool UIComponent::hitTest(int x, int y)
{
  if (((x > (int) m_x) && (x < (int) m_x + m_width)) &&
    ((y > (int) m_y) && (y < (int) m_y + m_height)))
  {
    return true;
  }
  return false;
}

void UIComponent::on(UIEvent::Type eventType, std::function<void(UIEvent)> handler)
{
  uiEventHandlers.insert(std::pair<UIEvent::Type, std::function<void(UIEvent)>>(eventType, handler));
}

void UIComponent::trigger(UIEvent ev)
{
  switch (ev.type)
  {
  case UIEvent::MouseMove:
    if (hitTest(ev.mouseX, ev.mouseY))
    {
      UIEvent mouseOver;
      mouseOver.type = UIEvent::MouseOver;
      mouseOver.mouseX = ev.mouseX;
      mouseOver.mouseY = ev.mouseY;
      m_mouseOver = true;
      trigger(mouseOver);
    }
    else if (!hitTest(ev.mouseX, ev.mouseY) && m_mouseOver)
    {
      UIEvent mouseOut;
      mouseOut.type = UIEvent::MouseOut;
      mouseOut.mouseX = ev.mouseX;
      mouseOut.mouseY = ev.mouseY;
      m_mouseOver = false;
      trigger(mouseOut);
    }
    break;
  }
  // fire user event handles
  if (uiEventHandlers.count(ev.type) > 0)
  {
    uiEventHandlers.at(ev.type)(ev);
  }
}

void UIComponent::setPosition(float x, float y)
{
  m_x = x;
  m_y = y;
}

void UIComponent::setSize(float width, float height)
{
  m_width = width;
  m_height = height;
}