#include "../Include/TextComponent.h"

TextComponent::TextComponent(void) :
  m_fontSize(24),
  m_r(0),
  m_g(0),
  m_b(0),
  m_text(L"")
{
}


TextComponent::~TextComponent(void)
{
}

void TextComponent::setColor(int r, int g, int b)
{
  m_r = r;
  m_g = g;
  m_b = b;
}

void TextComponent::setFontSize(int fontSize)
{
  m_fontSize = fontSize;
}

void TextComponent::setText(std::wstring text)
{
  m_text = text;
}

void TextComponent::paint(Gdiplus::Graphics& graphics)
{
  Gdiplus::SolidBrush brush(Gdiplus::Color(m_r, m_g, m_b));
  Gdiplus::FontFamily fontFamily(L"Helvetica");
  Gdiplus::Font font(&fontFamily, (Gdiplus::REAL) m_fontSize, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
  Gdiplus::PointF pointF(m_x, m_y);
  Gdiplus::RectF boundingBox;
  graphics.MeasureString(m_text.c_str(), -1, &font, pointF, &boundingBox);
  m_width = boundingBox.Width;
  m_height = boundingBox.Height;
  graphics.DrawString(m_text.c_str(), -1, &font, pointF, &brush);
}