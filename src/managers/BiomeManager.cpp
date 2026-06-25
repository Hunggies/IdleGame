#include "managers/BiomeManager.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>
#include <random>
#include <algorithm>

// Hàm phân tách chuỗi phụ trợ để cắt khoảng trắng thừa
static std::string trimSpace(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

bool BiomeManager::loadBiomeData(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "[Lỗi] Không thể mở file cấu hình Biome tại: " << filePath << std::endl;
        return false;
    }

    std::string line;
    BiomeData currentBiome;
    bool inBiome = false;

    while (std::getline(file, line)) {
        line = trimSpace(line);
        if (line.empty() || line[0] == '#') continue;

        if (line.rfind("BIOME_ID:", 0) == 0) {
            currentBiome = BiomeData();
            currentBiome.id = trimSpace(line.substr(9));
            inBiome = true;
        } else if (inBiome) {
            if (line.rfind("NAME:", 0) == 0) {
                currentBiome.name = trimSpace(line.substr(5));
            } else if (line.rfind("TYPE:", 0) == 0) {
                currentBiome.type = trimSpace(line.substr(5)); // FISHING hoặc FARMING
            } else if (line.rfind("LEVEL_REQ:", 0) == 0) {
                currentBiome.levelReq = std::stoi(trimSpace(line.substr(10)));
            } else if (line.rfind("BASE_AMOUNT:", 0) == 0) {
                currentBiome.baseAmount = std::stod(trimSpace(line.substr(12)));
            } else if (line.rfind("BASE_INTERVAL:", 0) == 0) {
                currentBiome.baseInterval = std::stod(trimSpace(line.substr(14)));
            } else if (line.rfind("LOOT_ITEM:", 0) == 0) {
                std::stringstream ss(line.substr(10));
                std::string itemId, chanceStr, rarityStr, expStr;
                
                std::getline(ss, itemId, ',');
                std::getline(ss, chanceStr, ',');
                std::getline(ss, rarityStr, ',');
                std::getline(ss, expStr, ',');

                BiomeItem item;
                item.id = trimSpace(itemId);
                item.baseChance = std::stod(trimSpace(chanceStr));
                item.rarityTier = std::stoi(trimSpace(rarityStr));
                item.expValue = std::stoll(trimSpace(expStr));
                
                currentBiome.lootTable.push_back(item);
            } else if (line == "END_BIOME") {
                biomeDatabase[currentBiome.id] = currentBiome;
                inBiome = false;
            }
        }
    }
    file.close();
    return true;
}

ActivityResult BiomeManager::executeActivity(const std::string& biomeId, Player& player, const CalculatedStats& stats) {
    ActivityResult result;
    result.success = false;
    result.foundChest = false;
    result.totalExpGained = 0;
    result.chestExp = 0;
    result.chestMoney = 0.0;

    // 1. Kiểm tra sự tồn tại của Biome trong bộ nhớ
    auto it = biomeDatabase.find(biomeId);
    if (it == biomeDatabase.end()) {
        result.errorMsg = "Vùng sinh thái không tồn tại!";
        return result;
    }
    const BiomeData& biome = it->second;
    result.biomeName = biome.name;

    // 2. Kiểm tra điều kiện Cấp độ người chơi (Player Level Requirements)
    if (player.level < biome.levelReq) {
        result.errorMsg = "Cấp độ hiện tại quá thấp! Yêu cầu cấp: " + std::to_string(biome.levelReq);
        return result;
    }

    // 3. Kiểm tra Cooldown thời gian thực
    if (player.cooldowns[biome.type] > 0) {
        result.errorMsg = "Hành động đang hồi chiêu! Còn lại: " + std::to_string(std::ceil(player.cooldowns[biome.type])) + "s";
        return result;
    }

    // Khởi tạo bộ sinh số ngẫu nhiên chuẩn C++11
    std::random_device rd;
    std::mt19937 gen(rd());

    // 4. Tính toán sản lượng ngẫu nhiên biến động từ 0.6x đến 1.4x theo thiết kế logic
    std::uniform_real_distribution<double> distRandomRange(0.6, 1.4);
    double randomMod = distRandomRange(gen);
    
    double statAmountMultiplier = (biome.type == "FISHING") ? stats.fishAmountMultiplier : stats.plantCountMultiplier;
    double finalBaseAmount = biome.baseAmount * statAmountMultiplier;
    
    // Tổng số lượng tài nguyên thu hoạch được trong lượt bấm này
    long long totalLootCount = static_cast<long long>(std::floor(finalBaseAmount * randomMod));
    if (totalLootCount < 0) totalLootCount = 0;

    // 5. Thuật toán dịch chuyển tỉ lệ dựa vào chỉ số Quality (Quality Scaling Logic)
    double qualityMultiplier = (biome.type == "FISHING") ? stats.fishQualityMultiplier : stats.plantQualityMultiplier;
    std::vector<double> adjustedChances(biome.lootTable.size(), 0.0);
    double accumulatedRareIncrease = 0.0;
    int commonItemIndex = -1;

    // Quét tìm vị trí vật phẩm Thường (Tier 0) và tính toán tỉ lệ mới tăng thêm cho các bậc Hiếm (Tier > 0)
    for (size_t i = 0; i < biome.lootTable.size(); ++i) {
        if (biome.lootTable[i].rarityTier == 0) {
            commonItemIndex = static_cast<int>(i);
        } else {
            double calculatedChance = biome.lootTable[i].baseChance * qualityMultiplier;
            adjustedChances[i] = calculatedChance;
            accumulatedRareIncrease += (calculatedChance - biome.lootTable[i].baseChance);
        }
    }

    // Thực hiện khấu trừ phần trăm tăng thêm trực tiếp vào ô của vật phẩm Thường
    if (commonItemIndex != -1) {
        double prospectiveCommonChance = biome.lootTable[commonItemIndex].baseChance - accumulatedRareIncrease;
        
        // Cơ chế phòng vệ biên: Nếu Quality quá cao làm sập tỉ lệ Common xuống âm, ép sàn tối thiểu 1%
        if (prospectiveCommonChance < 0.01) {
            double overflowDelta = 0.01 - prospectiveCommonChance;
            prospectiveCommonChance = 0.01;
            
            // Chia đều khấu trừ phần dư thừa tỉ lệ thuận ngược lại cho các vật phẩm hiếm
            double sumOfRareChances = 0.0;
            for (size_t i = 0; i < biome.lootTable.size(); ++i) {
                if (biome.lootTable[i].rarityTier > 0) sumOfRareChances += adjustedChances[i];
            }
            if (sumOfRareChances > 0.0) {
                for (size_t i = 0; i < biome.lootTable.size(); ++i) {
                    if (biome.lootTable[i].rarityTier > 0) {
                        adjustedChances[i] -= overflowDelta * (adjustedChances[i] / sumOfRareChances);
                    }
                }
            }
        }
        adjustedChances[commonItemIndex] = prospectiveCommonChance;
    } else {
        // Trong trường hợp Biome thiết kế lỗi không có vật phẩm tier 0, giữ nguyên tỉ lệ gốc
        for (size_t i = 0; i < biome.lootTable.size(); ++i) {
            adjustedChances[i] = biome.lootTable[i].baseChance;
        }
    }

    // 6. Phân phối số lượng và chuyển giao tài nguyên vào túi đồ Player (Lấy FLOOR)
    long long globalExpAwarded = 0;
    for (size_t i = 0; i < biome.lootTable.size(); ++i) {
        long long singleAllocatedAmount = static_cast<long long>(std::floor(totalLootCount * adjustedChances[i]));
        if (singleAllocatedAmount > 0) {
            result.gatheredItems[biome.lootTable[i].id] = singleAllocatedAmount;
            player.addResource("Basic", biome.lootTable[i].id, singleAllocatedAmount);
            globalExpAwarded += (singleAllocatedAmount * biome.lootTable[i].expValue);
        }
    }
    result.totalExpGained = globalExpAwarded;
    player.addEXP(globalExpAwarded);

    // 7. Logic bóc tách Gacha Rương kho báu (Treasure System Logic)
    double standardTreasureChance = 0.05; // Mặc định cơ bản là 5% nổ rương
    double updatedTreasureChance = standardTreasureChance * stats.treasureChanceMultiplier;
    std::uniform_real_distribution<double> distRollZeroOne(0.0, 1.0);

    if (distRollZeroOne(gen) < updatedTreasureChance) {
        result.foundChest = true;

        // Tính toán độ hiếm rương (Gốc: Wood 60%, Iron 25%, Gold 12%, Diamond 3%)
        double wWood = 0.60, wIron = 0.25, wGold = 0.12, wDiamond = 0.03;
        wIron *= stats.treasureQualityMultiplier;
        wGold *= stats.treasureQualityMultiplier;
        wDiamond *= stats.treasureQualityMultiplier;
        
        double shiftSum = (wIron - 0.25) + (wGold - 0.12) + (wDiamond - 0.03);
        wWood -= shiftSum;
        if (wWood < 0.01) wWood = 0.01; // Ép sàn rương gỗ 1%

        // Bình ổn hóa lại xác suất phân phối tổng rương đạt 1.0
        double renormalizeFactor = wWood + wIron + wGold + wDiamond;
        wWood /= renormalizeFactor; wIron /= renormalizeFactor; 
        wGold /= renormalizeFactor; wDiamond /= renormalizeFactor;

        double chestRollResult = distRollZeroOne(gen);
        int baseDropSlots = 2; // Số lượng vật phẩm rơi mặc định từ rương

        if (chestRollResult < wDiamond) {
            result.chestRarity = "Diamond";
            baseDropSlots = 5;
        } else if (chestRollResult < wDiamond + wGold) {
            result.chestRarity = "Gold";
            baseDropSlots = 4;
        } else if (chestRollResult < wDiamond + wGold + wIron) {
            result.chestRarity = "Iron";
            baseDropSlots = 3;
        } else {
            result.chestRarity = "Wood";
            baseDropSlots = 2;
        }

        // Tính tổng số lượt quay thưởng trong rương chịu ảnh hưởng từ chỉ số Số lượng (Treasure Item Quantity Multiplier)
        int calculatedTotalSlots = static_cast<int>(std::floor(baseDropSlots * stats.treasureItemQuantityMultiplier));
        if (calculatedTotalSlots < 1) calculatedTotalSlots = 1;

        std::uniform_int_distribution<int> distRewardCategory(1, 4);
        
        // Tiến hành quay xúc xắc rơi đồ cho từng Slot phần thưởng trong rương
        for (int step = 0; step < calculatedTotalSlots; ++step) {
            int selectedCategory = distRewardCategory(gen);
            
            if (selectedCategory == 1) { // Rơi EXP tỉ lệ thuận theo bình phương cấp độ hiện tại
                long long computedExpBonus = (std::max(1, player.level) * std::max(1, player.level)) * (std::uniform_int_distribution<int>(10, 50)(gen));
                result.chestExp += computedExpBonus;
                player.addEXP(computedExpBonus);
            } 
            else if (selectedCategory == 2) { // Rơi Tiền $ nhân hệ số thương nhân Sell Price Multiplier
                double computedCashBonus = (player.level + 1) * (std::uniform_int_distribution<int>(100, 500)(gen)) * stats.sellPriceMultiplier;
                result.chestMoney += computedCashBonus;
                player.addMoney(computedCashBonus);
            } 
            else if (selectedCategory == 3) { // Rơi Special Resource (Tài nguyên quý hiếm phân tách theo nghề)
                std::vector<std::string> specialPool;
                if (biome.type == "FISHING") {
                    specialPool = {"Golden Fish", "Emerald Fish", "Lava Fish", "Diamond Fish"};
                } else {
                    specialPool = {"Golden Potato", "Emerald Melon", "Sapphire Carrot", "Crystal Apple"};
                }
                std::uniform_int_distribution<size_t> indexDist(0, specialPool.size() - 1);
                std::string chosenResId = specialPool[indexDist(gen)];
                
                result.chestSpecialResources[chosenResId] += 1;
                player.addResource("Special", chosenResId, 1);
            } 
            else if (selectedCategory == 4) { // Rơi Charms bùa chú trực tiếp
                std::vector<std::string> charmsPool = {
                    "MarketingCharm", "EnduranceCharm", "QualityCharm", "LuckyCharm", 
                    "FortuneCharm", "GatheringCharm", "BuildingCharm", "ResearchingCharm", "ProducingCharm"
                };
                std::uniform_int_distribution<size_t> indexDist(0, charmsPool.size() - 1);
                std::string chosenCharmId = charmsPool[indexDist(gen)];
                
                result.chestCharms[chosenCharmId] += 1;
                player.charmLevels[chosenCharmId] += 1; // Cộng trực tiếp tăng cấp độ bùa
            }
        }
    }

    // 8. Kích hoạt thiết lập đếm ngược Cooldown của Biome sau hoạt động thành công
    player.cooldowns[biome.type] = biome.baseInterval;
    result.success = true;
    return result;
}