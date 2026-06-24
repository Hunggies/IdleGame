#pragma once

#include <vector>
#include <string>

#include "../models/BiomeDefinition.h"

class BiomeLoader {
public:

   std::vector<BiomeDefinition>
   Load(
      const std::string& filePath
   );
};