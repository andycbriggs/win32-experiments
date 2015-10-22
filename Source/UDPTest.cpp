#include <iostream>
#include <memory>
#include <string>

#include "Include\Application.h"

#include "Include\UDPSocket.h"

class UDPTest : public Application
{
public:
  void run() {

    std::shared_ptr<UDPSocket> client = std::make_shared<UDPSocket>();
    std::shared_ptr<UDPSocket> server = std::make_shared<UDPSocket>();

    server->bind("0.0.0.0", 41234);

    server->on(SocketEvent::Data, [=] (SocketEvent& ev) {
      std::cout << "received " << ev.data << std::endl;
    });

    client->on(SocketEvent::Error, [=] (SocketEvent& ev) {
      std::cout << "shit " << ev.error << std::endl;
    });

    client->poll();
    client->sendto(std::string("Hello World!"), "127.0.0.1", 41234);
    std::cout << "send" << std::endl;
    server->poll();
    client->sendto(std::string("Hello World!"), "127.0.0.1", 41234);
    std::cout << "send" << std::endl;
    server->poll();
    client->sendto(std::string("Hello World!"), "127.0.0.1", 41234);
    std::cout << "send" << std::endl;
    server->poll();
    client->sendto(std::string("Hello World!"), "127.0.0.1", 41234);
    std::cout << "send" << std::endl;
    server->poll();
    client->sendto(std::string("Hello World!"), "127.0.0.1", 41234);
    std::cout << "send" << std::endl;
    server->poll();

    std::cin.ignore();

  };

};