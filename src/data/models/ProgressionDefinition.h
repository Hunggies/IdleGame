#pragma once

#include <string>
#include <vector>

#include "ProgressionCategory.h"
#include "ProgressionLevelDefinition.h"

class ProgressionDefinition {
public:

   std::string id;

   std::string name;

   ProgressionCategory category;

   int maxLevel;

   std::vector<ProgressionLevelDefinition> levels;

   ProgressionDefinition();
};