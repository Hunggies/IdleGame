#pragma once

#include <string>

#include "RequirementType.h"

class RequirementDefinition {
public:

   RequirementType type;

   std::string targetId;

   int value;

   RequirementDefinition();
};