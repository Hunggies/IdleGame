#pragma once

#include <unordered_map>
#include <string>

class FishResult {
public:

   std::unordered_map<std::string, long long> rewards;

   double exp;

   bool treasureFound;

   FishResult();
};