#include <windows.h>
#include <string>

#include <stdio.h>

#include "Include/Window.h"
#include "Include/TextComponent.h"
#include "Include/ImageComponent.h"

#include "Include/TCPSocket.h"
#include "Include/TCPServerSocket.h"

class Server
{
  std::shared_ptr<Window> window;
  std::shared_ptr<TCPServerSocket> socket;

  std::vector<std::shared_ptr<TCPSocket>> clients;

public:
  void init()
  {
    window = std::make_shared<Window>();
    window->setTitle(L"Server!");
    window->setSize(640, 240);
    window->setBackgroundColor(0, 0, 40);
    window->show();

    std::shared_ptr<TextComponent> title = std::make_shared<TextComponent>();
    title->setPosition(10, 10);
    title->setFontSize(32);
    title->setColor(236, 240, 241);
    title->setText(L"Server");

    std::shared_ptr<TextComponent> votes = std::make_shared<TextComponent>();
    votes->setPosition(10, 100);
    votes->setFontSize(24);
    votes->setColor(236, 240, 241);
    votes->setText(L"Votes");

    socket = std::make_shared<TCPServerSocket>();
    socket->on(SocketEvent::Error, [=] (SocketEvent ev) {
      title->setText(std::to_wstring(ev.error));
    });
    socket->on(SocketEvent::Connection, [=] (SocketEvent ev) {
      clients.push_back(ev.socket);
      title->setText(std::to_wstring(clients.size()));
      ev.socket->on(SocketEvent::Data, [=] (SocketEvent ev) {
        std::wstring data;
        data.assign(ev.data.begin(), ev.data.end());
        votes->setText(data);
        for (auto client : clients) {
          client->send(ev.data);
        }
      });
    });
    socket->bind("0.0.0.0", 1337);
    socket->listen();

    title->on(UIEvent::MouseOver, [=] (UIEvent ev) {
      window->setTitle(L"MouseOver");
    });

    title->on(UIEvent::MouseOut, [=] (UIEvent ev) {
      window->setTitle(L"MouseOut");
    });

    window->addUIComponent(title);
    window->addUIComponent(votes);
  }
  void tick()
  {
    socket->poll();

    // poll all clients
    for (auto client : clients)
    {
      client->poll();
    }

    UIEvent ev;

    while(window->pollEvents(ev))
    {    
      // handle custom events, like close so we can save data before closing
    }
    window->paint();
  }
  bool isActive()
  {
    return window->isOpen();
  }
};

class Client
{
  std::shared_ptr<Window> window;
  std::shared_ptr<TCPSocket> socket;
  bool isConnected;
  int count;
public:
  void init()
  {
    isConnected = false;

    window = std::make_shared<Window>();
    window->setTitle(L"Client!");
    window->setSize(640, 240);
    window->show();

    std::shared_ptr<TextComponent> title = std::make_shared<TextComponent>();
    title->setPosition(10, 10);
    title->setFontSize(32);
    title->setColor(236, 240, 241);
    title->setText(L"Client");

    std::shared_ptr<TextComponent> direction = std::make_shared<TextComponent>();
    direction->setPosition(10, 100);
    direction->setFontSize(24);
    direction->setColor(236, 240, 241);
    direction->setText(L"Direction");

    std::shared_ptr<TextComponent> votes = std::make_shared<TextComponent>();
    votes->setPosition(10, 140);
    votes->setFontSize(24);
    votes->setColor(236, 240, 241);
    votes->setText(L"Votes");

    std::shared_ptr<ImageComponent> upvote = std::make_shared<ImageComponent>();
    upvote->loadImage(L"../Assets/upvote.png");
    upvote->setSize(100, 100);
    upvote->setPosition(500, 20);
    upvote->on(UIEvent::MouseOver, [=] (UIEvent ev) {
      window->setTitle(L"MouseOver");
      direction->setText(L"Up");
    });
    upvote->on(UIEvent::MouseDown, [=] (UIEvent ev) {
      if (isConnected) {
        socket->send(std::to_string(++count));
        votes->setText(std::to_wstring(count));
      }
    });

    std::shared_ptr<ImageComponent> downvote = std::make_shared<ImageComponent>();
    downvote->loadImage(L"../Assets/downvote.png");
    downvote->setSize(100, 100);
    downvote->setPosition(500, 130);
    downvote->on(UIEvent::MouseOver, [=] (UIEvent ev) {
      window->setTitle(L"MouseOver");
      direction->setText(L"Down");
    });
    downvote->on(UIEvent::MouseDown, [=] (UIEvent ev) {
      if (isConnected) {
        socket->send(std::to_string(--count));
        votes->setText(std::to_wstring(count));
      }
    });

    socket = std::make_shared<TCPSocket>();
    socket->on(SocketEvent::Error, [=] (SocketEvent ev) {
      if (!isConnected) return;
      isConnected = false;
      title->setText(std::to_wstring(ev.error));
    });
    socket->on(SocketEvent::Connected, [=] (SocketEvent ev) {
      title->setText(L"Socket Connected");
      isConnected = true;
    });
    socket->on(SocketEvent::Data, [=] (SocketEvent ev) {
      count = atoi(ev.data.c_str());
      votes->setText(std::to_wstring(count));
    });
    socket->connect("127.0.0.1", 1337);

    count = 0;

    title->on(UIEvent::MouseOver, [=] (UIEvent ev) {
      window->setTitle(L"MouseOver");
    });
    title->on(UIEvent::MouseOut, [=] (UIEvent ev) {
      window->setTitle(L"MouseOut");
    });

    window->addUIComponent(title);
    window->addUIComponent(upvote);
    window->addUIComponent(downvote);
    window->addUIComponent(direction);
    window->addUIComponent(votes);
  }
  void tick()
  {
    socket->poll();

    UIEvent ev;

    while (window->pollEvents(ev))
    {    
      // handle custom events, like close so we can save data before closing
    }
    window->paint();
  }
  bool isActive()
  {
    return window->isOpen();
  }
};

int main(int argc, char *argv[])
{

  Server server;

  std::vector<std::shared_ptr<Client>> clients;

  int numClients = 5;

  server.init();

  for (int i = 0; i < numClients; i++)
  {
    clients.push_back(std::make_shared<Client>());
  }

  for (auto client : clients)
  {
    client->init();
    server.tick();
  }

  while (server.isActive())
  {
    server.tick();
    for (auto client : clients)
    {
      client->tick();
      //if (!client->isActive()) return 0;
    }
    Sleep(0);
  }


  return 0;
}