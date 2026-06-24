#include "ProgressionLoader.h"

#include <fstream>

#include "../../utils/StringUtils.h"
#include "../parsers/ProgressionCategoryParser.h"

std::vector<ProgressionDefinition>
ProgressionLoader::Load(
   const std::string& filePath
) {

   std::vector<ProgressionDefinition>
      progressions;

   std::ifstream file(filePath);

   if (!file.is_open()) {
      return progressions;
   }

   ProgressionDefinition currentProgression;

   ProgressionLevelDefinition currentLevel;

   bool readingProgression = false;

   bool readingLevel = false;

   std::string line;

   while (
      std::getline(file, line)
   ) {

      line = StringUtils::Trim(line);

      if (line.empty()) {
         continue;
      }

      if (line == "[PROGRESSION]") {

         currentProgression =
            ProgressionDefinition();

         readingProgression = true;

         continue;
      }

      if (
         line.rfind("[level=", 0) == 0
      ) {

         if (readingLevel) {

            currentProgression.levels
               .push_back(currentLevel);
         }

         currentLevel =
            ProgressionLevelDefinition();

         std::string levelText =
            line.substr(
               7,
               line.length() - 8
            );

         currentLevel.level =
            std::stoi(levelText);

         readingLevel = true;

         continue;
      }

      if (line == "END") {

         if (readingLevel) {

            currentProgression.levels
               .push_back(currentLevel);
         }

         progressions.push_back(
            currentProgression
         );

         readingProgression = false;

         readingLevel = false;

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

      if (!readingLevel) {

         if (key == "id") {

            currentProgression.id =
               value;
         }

         else if (key == "name") {

            currentProgression.name =
               value;
         }

         else if (key == "category") {

            currentProgression.category =
               ProgressionCategoryParser::Parse(
                  value
               );
         }

         else if (key == "max_level") {

            currentProgression.maxLevel =
               std::stoi(value);
         }
      }

      else {

         if (key == "duration") {

            currentLevel.durationSeconds =
               std::stoi(value);
         }

         else if (key == "cost") {

            auto costParts =
               StringUtils::Split(
                  value,
                  ','
               );

            if (
               costParts.size() >= 2
            ) {

               ResourceCost cost;

               cost.resourceId =
                  costParts[0];

               cost.amount =
                  std::stoll(
                     costParts[1]
                  );

               currentLevel.costs
                  .push_back(cost);
            }
         }

         else if (
            key == "modifier"
         ) {

            auto modifierParts =
               StringUtils::Split(
                  value,
                  ','
               );

            if (
               modifierParts.size() >= 3
            ) {

               ModifierDefinition
                  modifier;

               /*
                * Chưa parse enum
                * tạm thời hardcode
                */

               modifier.value =
                  std::stod(
                     modifierParts[2]
                  );

               currentLevel.modifiers
                  .push_back(
                     modifier
                  );
            }
         }
      }
   }

   file.close();

   return progressions;
}