#pragma once

#include <string>

#include "ResourceType.h"

class ResourceDefinition {
public:

   int id;

   std::string name;

   ResourceType type;

   long long value;

   double exp;

   ResourceDefinition();
};