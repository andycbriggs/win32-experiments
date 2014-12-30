#pragma once
struct UIEvent {
  enum Type {
    MouseOver,
    MouseOut,
    MouseMove,
    MouseUp,
    MouseDown
  };
  Type type;
  int mouseX;
  int mouseY;
};
