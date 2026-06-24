#include "ProgressionCategoryParser.h"

ProgressionCategory
ProgressionCategoryParser::Parse(
   const std::string& text
) {

   if (text == "Building") {
      return ProgressionCategory::Building;
   }

   if (text == "Research") {
      return ProgressionCategory::Research;
   }

   if (text == "Charm") {
      return ProgressionCategory::Charm;
   }

   if (text == "Rod") {
      return ProgressionCategory::Rod;
   }

   if (text == "Hoe") {
      return ProgressionCategory::Hoe;
   }

   if (text == "Upgrade") {
      return ProgressionCategory::Upgrade;
   }

   if (text == "Special") {
      return ProgressionCategory::Special;
   }

   if (text == "Prestige") {
      return ProgressionCategory::Prestige;
   }

   return ProgressionCategory::Building;
}