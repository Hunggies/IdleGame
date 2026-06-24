#pragma once

#include <vector>

#include "ModifierDefinition.h"
#include "RequirementDefinition.h"
#include "ResourceCost.h"

class ProgressionLevelDefinition {
public:

   int level;

   int durationSeconds;

   std::vector<ResourceCost> costs;

   std::vector<ModifierDefinition> modifiers;

   std::vector<RequirementDefinition> requirements;

   ProgressionLevelDefinition();
};