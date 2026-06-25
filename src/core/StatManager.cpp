#include "core/StatManager.hpp"
#include <cmath>

CalculatedStats StatManager::calculateStats(const Player& player, const DataParser& parser) {
    CalculatedStats stats;

    // 1. Cộng dồn chỉ số từ hệ thống Bùa Chú (Charms) nhận được từ hòm Gacha
    for (const auto& pair : player.charmLevels) {
        std::string charmId = pair.first;
        int level = pair.second;
        if (level <= 0) continue;

        auto it = parser.charmDatabase.find(charmId);
        if (it != parser.charmDatabase.end()) {
            const CharmData& cData = it->second;
            for (const auto& buff : cData.buffsPerLevel) {
                std::string statType = buff.first;
                double valPerLevel = buff.second;
                double totalBuff = valPerLevel * level;

                if (statType == "SellPrice") stats.sellPriceMultiplier += totalBuff;
                else if (statType == "FishQuality") stats.fishQualityMultiplier += totalBuff;
                else if (statType == "FishAmount") stats.fishAmountMultiplier += totalBuff;
                else if (statType == "PlantQuality") stats.plantQualityMultiplier += totalBuff;
                else if (statType == "PlantCount") stats.plantCountMultiplier += totalBuff;
                else if (statType == "TreasureChance") stats.treasureChanceMultiplier += totalBuff;
                
                // === ĐÃ THÊM: Xử lý tăng số lượng vật phẩm rơi ra từ rương báu ===
                else if (statType == "TreasureQuantity") stats.treasureItemQuantityMultiplier += totalBuff;
                
                else if (statType == "PassiveGen") stats.passiveGenSpeedMultiplier += totalBuff;
            }
        }
    }

    // 2. Cộng dồn chỉ số tốc độ từ các Công trình (Facilities) đang sở hữu
    for (const auto& pair : player.facilityLevels) {
        std::string bId = pair.first;
        int level = pair.second;
        if (level <= 0) continue;

        auto it = parser.buildingDatabase.find(bId);
        if (it != parser.buildingDatabase.end()) {
            auto lvIt = it->second.levels.find(level);
            if (lvIt != it->second.levels.end()) {
                const LevelConfig& cfg = lvIt->second;
                for (const auto& buff : cfg.buffs) {
                    if (buff.first == "BuildingSpeedBonus") stats.buildingSpeedBonus += std::stod(buff.second);
                    else if (buff.first == "ResearchSpeedBonus") stats.researchSpeedBonus += std::stod(buff.second);
                }
            }
        }
    }

    // 3. Cộng dồn chỉ số từ Cây công nghệ (Research) trong Học viện
    for (const auto& pair : player.researchLevels) {
        std::string rId = pair.first;
        int level = pair.second;
        if (level <= 0) continue;

        auto it = parser.researchDatabase.find(rId);
        if (it != parser.researchDatabase.end()) {
            auto lvIt = it->second.levels.find(level);
            if (lvIt != it->second.levels.end()) {
                const LevelConfig& cfg = lvIt->second;
                for (const auto& buff : cfg.buffs) {
                    if (buff.first == "BuildingSpeedBonus") stats.buildingSpeedBonus += std::stod(buff.second);
                    else if (buff.first == "ResearchSpeedBonus") stats.researchSpeedBonus += std::stod(buff.second);
                    else if (buff.first == "SellPriceMultiplier") stats.sellPriceMultiplier += std::stod(buff.second);
                }
            }
        }
    }

    return stats;
}