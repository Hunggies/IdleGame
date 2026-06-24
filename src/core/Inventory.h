#pragma once

#include <unordered_map>
#include <string>

class Inventory {
public:

   std::unordered_map<std::string, long long> resources;

   void Add(
      const std::string& resourceId,
      long long amount
   );

   bool Remove(
      const std::string& resourceId,
      long long amount
   );

   long long GetAmount(
      const std::string& resourceId
   ) const;
};