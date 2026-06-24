#pragma once

#include <string>
#include <vector>

#include "LootEntryDefinition.h"

class LootTableDefinition {
public:

   std::string id;

   std::vector<LootEntryDefinition> entries;

   LootTableDefinition();
};