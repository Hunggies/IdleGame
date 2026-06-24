#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "Inventory.h"
#include "BaseStats.h"
#include "ModifierStats.h"
#include "ActiveEffect.h"
#include "ConstructionTask.h"
#include "ResearchTask.h"

class Player {
public:

   int level;

   double exp;

   long long balance;

   int rebirthCount;

   int prestigeCount;

   std::string currentBiomeId;

   Inventory inventory;

   BaseStats baseStats;

   ModifierStats modifierStats;

   std::vector<ActiveEffect> activeEffects;

   std::unordered_map<std::string, int> progressionLevels;

   Player();

   int GetProgressionLevel(
      const std::string& progressionId
   ) const;

   void SetProgressionLevel(
      const std::string& progressionId,
      int level
   );

   ConstructionTask constructionTask;

   ResearchTask researchTask;
};