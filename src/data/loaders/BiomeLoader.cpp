#include "BiomeLoader.h"

#include <fstream>

#include "../../utils/StringUtils.h"

std::vector<BiomeDefinition>
BiomeLoader::Load(
   const std::string& filePath
) {

   std::vector<BiomeDefinition> biomes;

   std::ifstream file(filePath);

   if (!file.is_open()) {
      return biomes;
   }

   BiomeDefinition current;

   std::string line;

   while (
      std::getline(file, line)
   ) {

      line =
         StringUtils::Trim(line);

      if (line.empty()) {
         continue;
      }

      if (line == "[BIOME]") {

         current =
            BiomeDefinition();

         continue;
      }

      if (line == "END") {

         biomes.push_back(
            current
         );

         continue;
      }

      auto parts =
         StringUtils::Split(
            line,
            '='
         );

      if (parts.size() < 2) {
         continue;
      }

      std::string key =
         parts[0];

      std::string value =
         parts[1];

      if (key == "id") {

         current.id = value;
      }

      else if (key == "name") {

         current.name = value;
      }

      else if (
         key ==
         "fish_amount_multiplier"
      ) {

         current.fishAmountMultiplier =
            std::stod(value);
      }

      else if (
         key ==
         "plant_count_multiplier"
      ) {

         current.plantCountMultiplier =
            std::stod(value);
      }

      else if (
         key ==
         "fishing_interval_multiplier"
      ) {

         current.fishingIntervalMultiplier =
            std::stod(value);
      }

      else if (
         key ==
         "farming_interval_multiplier"
      ) {

         current.farmingIntervalMultiplier =
            std::stod(value);
      }

      else if (
         key ==
         "treasure_chance_multiplier"
      ) {

         current.treasureChanceMultiplier =
            std::stod(value);
      }

      else if (key == "fish") {

         auto fishParts =
            StringUtils::Split(
               value,
               ','
            );

         if (
            fishParts.size() >= 2
         ) {

            BiomeDrop drop;

            drop.resourceId =
               fishParts[0];

            drop.weight =
               std::stod(
                  fishParts[1]
               );

            current.fishDrops
               .push_back(drop);
         }
      }

      else if (key == "plant") {

         auto plantParts =
            StringUtils::Split(
               value,
               ','
            );

         if (
            plantParts.size() >= 2
         ) {

            BiomeDrop drop;

            drop.resourceId =
               plantParts[0];

            drop.weight =
               std::stod(
                  plantParts[1]
               );

            current.plantDrops
               .push_back(drop);
         }
      }
   }

   file.close();

   return biomes;
}