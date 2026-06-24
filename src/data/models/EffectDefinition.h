#pragma once

#include <string>
#include <vector>

#include "ModifierDefinition.h"

class EffectDefinition {
public:

   std::string id;

   std::string name;

   int durationSeconds;

   std::vector<ModifierDefinition>
      modifiers;

   EffectDefinition();
};