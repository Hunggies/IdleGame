#pragma once

#include "../core/Player.h"
#include "../core/FishResult.h"
#include "../data/database/GameDatabase.h"

class FishSystem {
public:

   FishResult Fish(
      Player& player,
      const GameDatabase& database
   );
};