#pragma once

#include <string>
#include <vector>

#include "BiomeDrop.h"

class BiomeDefinition {
public:

   std::string id;

   std::string name;

   double fishAmountMultiplier;

   double plantCountMultiplier;

   double fishingIntervalMultiplier;

   double farmingIntervalMultiplier;

   double treasureChanceMultiplier;

   std::vector<BiomeDrop> fishDrops;

   std::vector<BiomeDrop> plantDrops;

   BiomeDefinition();
};