#pragma once

#include "ModifierType.h"
#include "ModifierMode.h"

class ModifierDefinition {
public:

   ModifierType type;

   ModifierMode mode;

   double value;

   ModifierDefinition();
};