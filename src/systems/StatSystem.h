#pragma once

#include "../core/Player.h"
#include "../data/database/GameDatabase.h"

class StatSystem {
public:

   void RecalculateStats(
      Player& player,
      const GameDatabase& database
   );
};