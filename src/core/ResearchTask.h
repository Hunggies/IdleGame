#pragma once

#include <string>

class ResearchTask {
public:

   bool active;

   std::string progressionId;

   double remainingSeconds;

   ResearchTask();
};