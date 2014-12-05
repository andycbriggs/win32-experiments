#include <windows.h>
#include <string>

#include "Window.h"
#include "TextComponent.h"
#include "ImageComponent.h"

int main(int argc, char *argv[])
{

  Window window = Window();
  window.setTitle(L"Sketchbook!");
  window.setSize(640, 240);
  window.show();

  std::shared_ptr<TextComponent> title = std::make_shared<TextComponent>();
  title->setPosition(10, 10);
  title->setFontSize(32);
  title->setColor(236, 240, 241);
  title->setText(L"Hello World! 你好世界！");
  title->on(UIEvent::MouseOver, [&window, title] (UIEvent ev) {
    window.setTitle(L"Woah shit!");
  });
  title->on(UIEvent::MouseOut, [&window, title] (UIEvent ev) {
    window.setTitle(L"Awwww :(");
  });
  window.addUIComponent(title);

  std::shared_ptr<TextComponent> subtitle = std::make_shared<TextComponent>();
  subtitle->setPosition(10, 50);
  subtitle->setFontSize(24);
  subtitle->setColor(189, 195, 199);
  subtitle->setText(L"What is Brian drinking?");
  window.addUIComponent(subtitle);

  std::shared_ptr<ImageComponent> image = std::make_shared<ImageComponent>();
  image->setPosition(420, 20);
  image->setSize(160, 200);
  image->loadImage(L"../Assets/brian.png");
  image->on(UIEvent::MouseOver, [image, subtitle] (UIEvent ev) {
    image->loadImage(L"../Assets/brian2.png");
    subtitle->setText(L"Brian is drinking wine");
  });
  image->on(UIEvent::MouseOut, [image, subtitle] (UIEvent ev) {
    image->loadImage(L"../Assets/brian.png");
    subtitle->setText(L"Brian is drinking champagne");
  });
  window.addUIComponent(image);


  UIEvent ev;

  while (window.isOpen())
  {
    while(window.pollEvents(ev))
    {    
      // handle custom events, like close so we can save data before closing
    }
    window.paint();
    Sleep(1);
  }

  return 0;
}