#include "ImageComponent.h"

ImageComponent::ImageComponent(void)
{
}


ImageComponent::~ImageComponent(void)
{
}

void ImageComponent::paint(Gdiplus::Graphics& graphics)
{
  graphics.DrawImage(m_image.get(), m_x, m_y, m_width, m_height);
}

void ImageComponent::loadImage(std::wstring path)
{
  m_image = std::shared_ptr<Gdiplus::Image>(Gdiplus::Image::FromFile(path.c_str()));
}