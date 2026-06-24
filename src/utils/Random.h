#pragma once

#include <vector>

class Random {
public:

   static int Int(
      int min,
      int max
   );

   static double Double(
      double min,
      double max
   );

   static int WeightedIndex(
      const std::vector<double>& weights
   );
};