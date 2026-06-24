#pragma once

#include <string>
#include <vector>

#include "BiomeEntryDefinition.h"

class BiomeDefinition {
public:

   std::string id;

   std::string name;

   std::vector<BiomeEntryDefinition> fishEntries;

   std::vector<BiomeEntryDefinition> plantEntries;

   double fishAmountMultiplier;

   double plantCountMultiplier;

   double treasureChanceMultiplier;

   double fishingIntervalMultiplier;

   double farmingIntervalMultiplier;

   BiomeDefinition();
};