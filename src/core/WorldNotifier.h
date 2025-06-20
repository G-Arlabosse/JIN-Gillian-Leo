#pragma once

#include "Constants.h"

class WorldNotifier {
 public:
  virtual ~WorldNotifier() = default;
  virtual void notifyTransition(direction dir) = 0;
  virtual void loadLobby() = 0;
};