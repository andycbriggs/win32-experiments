# afaik

A sketchbook of approaches to working with the win32 api using modern c++.

## Why?

Experimentation, curiosity and some motivation to make writing native win32 applications fun and expressive.

The following snippet is from an early test:

```cpp
std::shared_ptr<Window> window = std::make_shared<Window>();
window->setTitle(L"Server!");
window->show();

std::shared_ptr<TextComponent> status = std::make_shared<TextComponent>();
status->setPosition(10, 10);
status->setFontSize(32);
status->setColor(236, 240, 241);
status->setText(L"Ready!");

window->addUIComponent(status);

std::shared_ptr<TCPSocket> socket = std::make_shared<TCPSocket>();
socket->on(SocketEvent::Error, [=] (SocketEvent ev) {
  status->setText(ev.error);
});
socket->on(SocketEvent::Connection, [=] (SocketEvent ev) {
  status->setText(L"Connection Received");
  clients.push_back(ev.socket);
  ev.socket->on(SocketEvent::Data, [=] (SocketEvent ev) {
    status->setText(data);
  });
});
socket->bind("127.0.0.1", 1337);
socket->listen();
```

