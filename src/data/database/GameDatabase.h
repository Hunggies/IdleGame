#pragma once

#include <unordered_map>
#include <string>

#include "../models/ResourceDefinition.h"
#include "../models/BiomeDefinition.h"
#include "../models/ProgressionDefinition.h"
#include "../models/TreasureDefinition.h"
#include "../models/LootTableDefinition.h"
#include "../models/EffectDefinition.h"

class GameDatabase {
public:

   std::unordered_map<
      std::string,
      ResourceDefinition
   > resources;

   std::unordered_map<
      std::string,
      BiomeDefinition
   > biomes;

   std::unordered_map<
      std::string,
      ProgressionDefinition
   > progressions;

   std::unordered_map<
      std::string,
      TreasureDefinition
   > treasures;

   std::unordered_map<
      std::string,
      LootTableDefinition
   > lootTables;

   std::unordered_map<
      std::string,
      EffectDefinition
   > effects;
};