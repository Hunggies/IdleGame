#pragma once

#include <string>

class ConstructionTask {
public:

   bool active;

   std::string progressionId;

   double remainingSeconds;

   ConstructionTask();
};