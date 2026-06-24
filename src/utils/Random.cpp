#include "Random.h"

#include <random>

static std::random_device rd;

static std::mt19937 rng(rd());

int Random::Int(
   int min,
   int max
) {

   std::uniform_int_distribution<int>
      dist(min, max);

   return dist(rng);
}

double Random::Double(
   double min,
   double max
) {

   std::uniform_real_distribution<double>
      dist(min, max);

   return dist(rng);
}

int Random::WeightedIndex(
   const std::vector<double>& weights
) {

   double totalWeight = 0.0;

   for (double weight : weights) {
      totalWeight += weight;
   }

   double roll =
      Double(0.0, totalWeight);

   double current = 0.0;

   for (size_t i = 0;
      i < weights.size();
      i++) {

      current += weights[i];

      if (roll <= current) {
         return static_cast<int>(i);
      }
   }

   return static_cast<int>(
      weights.size() - 1
   );
}