#pragma once

struct UIEvent {
  enum Type {
    WM,
    MouseOver,
    MouseOut,
    MouseMove,
    MouseUp,
    MouseDown
  };
  Type type;
  int wmMessage;
  int mouseX;
  int mouseY;
};
