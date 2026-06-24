#pragma once

#include <string>
#include <vector>

#include "../models/ProgressionDefinition.h"

class ProgressionLoader {
public:

   std::vector<ProgressionDefinition>
   Load(
      const std::string& filePath
   );
};