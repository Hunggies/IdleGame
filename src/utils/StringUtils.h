#pragma once

#include <string>
#include <vector>

class StringUtils {
public:

   static std::string Trim(
      const std::string& str
   );

   static std::vector<std::string> Split(
      const std::string& str,
      char delimiter
   );
};