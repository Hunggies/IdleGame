#include "Inventory.h"

void Inventory::Add(
   const std::string& resourceId,
   long long amount
) {
   resources[resourceId] += amount;
}

bool Inventory::Remove(
   const std::string& resourceId,
   long long amount
) {
   if (GetAmount(resourceId) < amount) {
      return false;
   }

   resources[resourceId] -= amount;

   return true;
}

long long Inventory::GetAmount(
   const std::string& resourceId
) const {

   auto it = resources.find(resourceId);

   if (it == resources.end()) {
      return 0;
   }

   return it->second;
}