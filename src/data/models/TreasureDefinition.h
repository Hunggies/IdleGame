#pragma once

#include "TreasureRarity.h"

class TreasureDefinition {
public:

   TreasureRarity rarity;

   double weight;

   int minDrops;

   int maxDrops;

   TreasureDefinition();
};