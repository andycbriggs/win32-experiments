#pragma once

#include "NonCopyable.h"

class Application : public NonCopyable {
  virtual void run() = 0;
};