#pragma once

#include <string>

#include "../models/ProgressionCategory.h"

class ProgressionCategoryParser {
public:

   static ProgressionCategory Parse(
      const std::string& text
   );
};