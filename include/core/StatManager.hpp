#ifndef STAT_MANAGER_HPP
#define STAT_MANAGER_HPP

#include "core/Player.hpp"
#include "managers/DataParser.hpp"

struct CalculatedStats {
    double fishQualityMultiplier = 1.0;
    double fishAmountMultiplier = 1.0;
    double plantQualityMultiplier = 1.0;
    double plantCountMultiplier = 1.0;
    double buildingSpeedBonus = 0.0;
    double researchSpeedBonus = 0.0;
    double treasureChanceMultiplier = 1.0;
    double treasureQualityMultiplier = 1.0;
    
    // === THÊM BIẾN NÀY VÀO ĐỂ SỬA LỖI BIOMEMANAGER ===
    double treasureItemQuantityMultiplier = 1.0; 
    
    double sellPriceMultiplier = 1.0;
    double passiveGenSpeedMultiplier = 1.0;
};

class StatManager {
public:
    CalculatedStats calculateStats(const Player& player, const DataParser& parser);
};

#endif // STAT_MANAGER_HPP