
#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>

#include "Include\Application.h"
#include "Include\TCPSocket.h"
#include "Include\TCPServerSocket.h"
#include "Include\SocketEvent.h"

class EchoServer : public Application
{
  TCPServerSocket server;
  std::vector<std::shared_ptr<TCPSocket>> clients;
  std::vector<std::shared_ptr<TCPSocket>>::iterator clientsIt;
  
public:
  void run () {

    std::cout << "starting" << std::endl;

    server.on(SocketEvent::Connection, [=] (SocketEvent& ev) {
      
      clients.push_back(ev.socket);

      std::cout << "connection received, count: " << clients.size() << std::endl;

      ev.socket->on(SocketEvent::Data, [=] (SocketEvent& ev) {
        std::cout << "data received: " << ev.data << std::endl;
        ev.socket->send(ev.data, ev.dataLength);
      });

      ev.socket->on(SocketEvent::Error, [=] (SocketEvent& ev) {
        std::cout << "socket error: " << ev.error << std::endl;
        ev.socket->close();
      });

      ev.socket->on(SocketEvent::Close, [=] (SocketEvent& ev) {
        clientsIt = clients.erase(std::remove(clients.begin(), clients.end(), ev.socket), clients.end());
        std::cout << "socket closed, count: " << clients.size() << std::endl;
      });

    });

    server.bind("0.0.0.0", 1337);
    server.listen();

    while (true) {
      server.poll();
      clientsIt = clients.begin();
      while (clientsIt != clients.end())
      {
        clientsIt->get()->poll();
        if (clientsIt != clients.end()) clientsIt++;
      }
    }

  };

};