
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

public:
  void run () {

    std::cout << "starting" << std::endl;

    server.on(SocketEvent::Connection, [=] (SocketEvent& ev) {

      std::cout << "connection received" << std::endl;
      
      clients.push_back(ev.socket);

      ev.socket->on(SocketEvent::Data, [=] (SocketEvent& ev) {
        std::cout << "data received: " << ev.data << std::endl;
        ev.socket->send(ev.data, ev.dataLength);
      });

      ev.socket->on(SocketEvent::Error, [=] (SocketEvent& ev) {
        std::cout << "socket error: " << ev.error << std::endl;
        clients.erase(std::remove(clients.begin(), clients.end(), ev.socket), clients.end());
      });

    });

    server.bind("0.0.0.0", 1337);
    server.listen();

    while (true) {
      server.poll();
      for (auto client : clients) {
        client->poll();
      }
    }

  };

};