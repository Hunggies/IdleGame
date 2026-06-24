#pragma once

#include "../core/Player.h"

class GameLoop {
public:

   void Update(
      Player& player,
      double deltaTime
   );
};