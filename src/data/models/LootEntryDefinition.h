#pragma once

#include <string>

class LootEntryDefinition {
public:

   std::string rewardId;

   double weight;

   int minAmount;

   int maxAmount;

   LootEntryDefinition();
};