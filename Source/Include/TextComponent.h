#pragma once

#include "UIComponent.h"
#include <string>

class TextComponent :
  public UIComponent
{
public:
  TextComponent(void);
  virtual ~TextComponent(void);
  virtual void paint(Gdiplus::Graphics& graphics);
  void setText(std::wstring text);
  void setColor(int r, int g, int b);
  void setFontSize(int fontSize);
private:
  std::wstring m_text;
  int m_r;
  int m_g;
  int m_b;
  int m_fontSize;
};

