#include "StringUtils.h"

#include <sstream>

std::string StringUtils::Trim(
   const std::string& str
) {

   size_t start =
      str.find_first_not_of(" \t\r\n");

   if (start == std::string::npos) {
      return "";
   }

   size_t end =
      str.find_last_not_of(" \t\r\n");

   return str.substr(
      start,
      end - start + 1
   );
}

std::vector<std::string>
StringUtils::Split(
   const std::string& str,
   char delimiter
) {

   std::vector<std::string> result;

   std::stringstream ss(str);

   std::string token;

   while (
      std::getline(
         ss,
         token,
         delimiter
      )
   ) {
      result.push_back(
         Trim(token)
      );
   }

   return result;
}