#include "Player.h"

Player::Player() {

   level = 0;

   exp = 0.0;

   balance = 0;

   rebirthCount = 0;

   prestigeCount = 0;

   currentBiomeId = "default";
}

int Player::GetProgressionLevel(
   const std::string& progressionId
) const {

   auto it =
      progressionLevels.find(progressionId);

   if (it == progressionLevels.end()) {
      return 0;
   }

   return it->second;
}

void Player::SetProgressionLevel(
   const std::string& progressionId,
   int level
) {
   progressionLevels[progressionId] = level;
}