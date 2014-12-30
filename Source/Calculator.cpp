
#include "Include\Window.h"

class Calculator
{
  Window window;
public:
  void run()
  {
    setup();
    loop();
    shutdown();
  }
private:
  void setup()
  {
    window.setTitle(L"AFAIK Calculator");
    window.loadIcon(L"../Assets/calculator.ico");
    window.show();
  }

  void loop()
  {
    UIEvent ev;
    while (window.isOpen())
    {
      while(window.pollEvents(ev))
      {

      }
    }
  }

  void shutdown()
  {

  }
};

int main(int argc, char *argv[])
{

  Calculator app;
  app.run();

  return 0;
}