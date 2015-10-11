#pragma once

#include "UIComponent.h"
#include <string>
#include <memory>

class ImageComponent :
  public UIComponent
{
public:
  ImageComponent(void);
  virtual ~ImageComponent(void);
  virtual void paint(Gdiplus::Graphics& graphics);
  virtual void loadImage(std::wstring path);
private:
  std::shared_ptr<Gdiplus::Image> m_image;
};

