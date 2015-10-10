#pragma once

#include <functional>
#include <map>

template <typename Event, typename EventType>
class EventEmitter
{
public:
  void trigger(Event& ev)
  {
    auto it = _eventHandlers.find(ev.type);
    if (it != _eventHandlers.end()) it->second(ev);
  };

  void on(EventType eventType, std::function<void(Event&)> handler)
  {
    _eventHandlers.insert(std::pair<EventType, std::function<void(Event&)>>(eventType, handler));
  };

  virtual void poll() = 0;
protected:
  std::map<EventType, std::function<void(Event&)>> _eventHandlers;
};