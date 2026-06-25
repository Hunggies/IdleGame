#include "ui/MenuRender.hpp"
#include <iostream>
#include <iomanip>
#include <cmath>

void MenuRender::clearScreen() {
#if defined(_WIN32) || defined(_WIN64)
    std::system("cls");
#else
    std::system("clear");
#endif
}

void MenuRender::drawDivider() {
    std::cout << "-------------------------------------------------------------\n";
}

// [source 1] Màn hình chính Home UI hiển thị Hồ sơ và Túi đồ vật phẩm
void MenuRender::renderHome(const Player& player, const CalculatedStats& stats) {
    drawDivider();
    std::cout << "Player Name: " << player.name << "\n\n";
   std::cout << "Level: " << player.level << " (EXP: " << player.exp << "/" << player.getNextLevelEXP() << ")\n";
   std::cout << "Balance: $" << std::fixed << std::setprecision(2) << player.balance << "\n\n";

    std::cout << "Inventory:\n";
    std::cout << "- Basic: (Fish, Farm)\n";
    if (player.inventoryBasic.empty()) std::cout << "  (Trống)\n";
    for (const auto& item : player.inventoryBasic) {
        std::cout << "  " << item.first << ": " << item.second << "\n";
    }

    std::cout << "\n- Special: (Golden Fish,...)\n";
    if (player.inventorySpecial.empty()) std::cout << "  (Trống)\n";
    for (const auto& item : player.inventorySpecial) {
        std::cout << "  " << item.first << ": " << item.second << "\n";
    }

    std::cout << "\n- Material: (Food, Wood,...)\n";
    if (player.inventoryMaterial.empty()) std::cout << "  (Trống)\n";
    for (const auto& item : player.inventoryMaterial) {
        std::cout << "  " << item.first << ": " << item.second << "\n";
    }

    // [source 2] Hiển thị các chỉ số nội tại nhân vật sau khi tính toán buff
    std::cout << "\nStats:\n";
    std::cout << "- Fish Quality / Amount Multiplier: " << stats.fishQualityMultiplier << "x / " << stats.fishAmountMultiplier << "x\n";
    std::cout << "- Plant Quality / Count Multiplier: " << stats.plantQualityMultiplier << "x / " << stats.plantCountMultiplier << "x\n";
    std::cout << "- Base Work Speed Bonus (Build/Research): +" << (stats.buildingSpeedBonus * 100) << "% / +" << (stats.researchSpeedBonus * 100) << "%\n";
    std::cout << "- Treasure Chance / Quality: " << stats.treasureChanceMultiplier << "x / " << stats.treasureQualityMultiplier << "x\n";
    std::cout << "- Market Sell Price Multiplier: " << stats.sellPriceMultiplier << "x\n";
    drawDivider();
    std::cout << "[1] Fish (Fish UI)\n";
    std::cout << "[2] Farm (Farm UI)\n";
    std::cout << "[3] Shop\n";
    std::cout << "[4] Menu\n";
    drawDivider();
}

// [source 3-6] Giao diện kết quả Câu cá thành công và nổ Rương báu
void MenuRender::renderFishUI(const ActivityResult& result) {
    drawDivider();
    std::cout << "Vùng đất: " << result.biomeName << "\n";
    std::cout << "You catched:\n";
    for (const auto& item : result.gatheredItems) {
        std::cout << "- " << item.first << ": " << item.second << "\n";
    }
    std::cout << "\n+" << result.totalExpGained << " EXP!\n\n";

    // Chỉ hiển thị khối này nếu may mắn nổ trúng rương gacha [source 4]
    if (result.foundChest) {
        std::cout << "You have found a <" << result.chestRarity << "> chest! |\n";
        std::cout << "You opened the crate and found:  |\n";
        if (result.chestExp > 0) std::cout << "+ " << result.chestExp << " EXP\n";
        if (result.chestMoney > 0) std::cout << "+ $" << std::fixed << std::setprecision(2) << result.chestMoney << "\n";
        for (const auto& res : result.chestSpecialResources) {
            std::cout << "+ " << res.second << " " << res.first << "\n";
        }
        for (const auto& charm : result.chestCharms) {
            std::cout << "+ " << charm.second << " " << charm.first << " Upgraded!\n";
        }
    }
    drawDivider();
    std::cout << "[0] Home\n";
    std::cout << "[1] Fish again\n";
    drawDivider();
}

// [source 7] Màn hình chặn câu cá do đang Cooldown
void MenuRender::renderFishCooldown(double timeLeft) {
    drawDivider();
    std::cout << "You are still in cooldown: " << std::fixed << std::setprecision(1) << timeLeft << "s left!\n";
    drawDivider();
    std::cout << "[0] Home\n";
    std::cout << "[1] Fish again\n";
    drawDivider();
}

// [source 9-11] Giao diện kết quả Trồng trọt gặt hái thành công và nổ Rương báu
void MenuRender::renderFarmUI(const ActivityResult& result) {
    drawDivider();
    std::cout << "Vùng đất: " << result.biomeName << "\n";
    std::cout << "You farmed:\n";
    for (const auto& item : result.gatheredItems) {
        std::cout << "- " << item.first << ": " << item.second << "\n";
    }
    std::cout << "\n+" << result.totalExpGained << " EXP!\n\n";

    if (result.foundChest) {
        std::cout << "You have found a <" << result.chestRarity << "> chest! |\n";
        std::cout << "You opened the crate and found:  |\n";
        if (result.chestExp > 0) std::cout << "+ " << result.chestExp << " EXP\n";
        if (result.chestMoney > 0) std::cout << "+ $" << std::fixed << std::setprecision(2) << result.chestMoney << "\n";
        for (const auto& res : result.chestSpecialResources) {
            std::cout << "+ " << res.second << " " << res.first << "\n";
        }
        for (const auto& charm : result.chestCharms) {
            std::cout << "+ " << charm.second << " " << charm.first << " (Đã nâng cấp)\n";
        }
    }
    drawDivider();
    std::cout << "[0] Home\n";
    std::cout << "[1] Farm again\n";
    drawDivider();
}

// [source 8] Màn hình chặn làm nông do đang Cooldown
void MenuRender::renderFarmCooldown(double timeLeft) {
    drawDivider();
    std::cout << "You are still in cooldown: " << std::fixed << std::setprecision(1) << timeLeft << "s left!\n";
    drawDivider();
    std::cout << "[0] Home\n";
    std::cout << "[1] Farm again\n";
    drawDivider();
}

// Giao diện lựa chọn các phân khu Cửa hàng
void MenuRender::renderShopMain() {
    drawDivider();
    std::cout << "[0] Home\n";
    std::cout << "[1] Rod Shop\n";
    std::cout << "[2] Hoe Shop\n";
    std::cout << "[3] Upgrade Shop\n";
    std::cout << "[4] Special Shop\n";
    std::cout << "[5] Boost Shop\n";
    std::cout << "[6] Speed Up Shop\n";
    std::cout << "[7] Prestige Shop\n";
    drawDivider();
}

// Chi tiết danh sách vật phẩm bày bán trong từng Category của Shop
void MenuRender::renderShopCategory(const std::string& categoryName, const ShopCategoryData& categoryData) {
    drawDivider();
    std::cout << " CHỢ GIAO DỊCH - PHÂN KHU: " << categoryName << "\n";
    drawDivider();
    
    int index = 1;
    for (const auto& item : categoryData.items) {
        std::cout << "[" << index++ << "] " << item.name << " (" << item.id << ")\n";
        std::cout << "    Mô tả: " << item.description << "\n";
        std::cout << "    Price:\n";
        for (const auto& p : item.price) {
            std::cout << "    - " << p.first << ": " << p.second << "\n";
        }
        std::cout << "\n";
    }
    drawDivider();
    std::cout << "[0] Return (to Shop UI)\n";
    drawDivider();
}

// Màn hình lựa chọn chức năng hệ thống nâng cao [source 4]
void MenuRender::renderMenuMain(const Player& player) {
    drawDivider();
    std::cout << "[0] Home\n";
    std::cout << "[1] Quest\n";
    std::cout << "[2] Charms\n";
    std::cout << "[3] Biome\n";
    std::cout << "[4] Build\n";
    
    // Điều kiện ẩn/hiện chuẩn GDD: Chỉ mở khóa khi Học viện Academy đã được xây [source 4]
    auto it = player.facilityLevels.find("Academy");
    if (it != player.facilityLevels.end() && it->second > 0) {
        std::cout << "[5] Research\n";
    } else {
        std::cout << "[5] Research (Khóa - Yêu cầu xây dựng Học viện Academy trước)\n";
    }
    drawDivider();
}

// Bảng nhiệm vụ tuần hoàn
void MenuRender::renderQuests() {
    drawDivider();
    std::cout << "Quest 1: Thợ câu tập sự\n";
    std::cout << "- Task: Câu thành công 10 con cá sông\n";
    std::cout << "- Reward: Wood: 50, Food: 50\n\n";
    std::cout << "Quest 2: Đại gia điền chủ\n";
    std::cout << "- Task: Sở hữu $10000 tiền mặt\n";
    std::cout << "- Reward: MarketingCharm: 1\n";
    drawDivider();
    std::cout << "[0] Return (to Menu)\n";
    drawDivider();
}

// Hiển thị danh sách bùa chú tâm linh nhận từ Rương Gacha
void MenuRender::renderCharms(const Player& player, const DataParser& parser) {
    drawDivider();
    std::cout << " CHARM LIST\n";
    drawDivider();
    
    for (const auto& dbPair : parser.charmDatabase) {
        std::string cId = dbPair.first;
        const CharmData& cData = dbPair.second;
        
        int currentLv = 0;
        auto pIt = player.charmLevels.find(cId);
        if (pIt != player.charmLevels.end()) currentLv = pIt->second;

        std::cout << cData.name << ": " << cData.description << "\n";
        std::cout << "- Level: " << currentLv << "\n";
        std::cout << "- Hệ số buff hiện tại: ";
        for (const auto& b : cData.buffsPerLevel) {
            std::cout << b.first << " (+" << (b.second * currentLv * 100) << "%) ";
        }
        std::cout << "\n\n";
    }
    drawDivider();
    std::cout << "[0] Return (to Menu)\n";
    drawDivider();
}

// [source 12] Màn hình chọn Vùng đất sinh thái để cắm chốt hoạt động
void MenuRender::renderBiomes(const BiomeManager& biomeManager) {
    drawDivider();
    std::cout << "Fishing Biomes:\n";
    for (const auto& pair : biomeManager.biomeDatabase) {
        if (pair.second.type == "FISHING") {
            std::cout << "- ID [" << pair.first << "]: " << pair.second.name << "\n";
            std::cout << "  + Level Requirement: " << pair.second.levelReq << "\n";
        }
    }

    std::cout << "\nFarming Biomes:\n";
    for (const auto& pair : biomeManager.biomeDatabase) {
        if (pair.second.type == "FARMING") {
            std::cout << "- ID [" << pair.first << "]: " << pair.second.name << "\n";
            std::cout << "  + Level Requirement: " << pair.second.levelReq << "\n";
        }
    }
    drawDivider();
    std::cout << "[0] Return (to Menu)\n";
    std::cout << "-> Type Biome ID (eg. River, GreenField)\n";
    drawDivider();
}

// Quản lý hiển thị công trình, điều kiện nâng cấp và hàng đợi xây dựng thời gian thực
void MenuRender::renderBuild(const Player& player, const DataParser& parser, const FacilityManager& facilityManager) {
    drawDivider();
    std::cout << " CONSTRUCTION\n";
    drawDivider();

    for (const auto& pair : parser.buildingDatabase) {
        std::string bId = pair.first;
        const BuildingData& bData = pair.second;

        int currentLv = 0;
        auto pIt = player.facilityLevels.find(bId);
        if (pIt != player.facilityLevels.end()) currentLv = pIt->second;

        std::cout << bData.name << " [" << bId << "]\n";
        std::cout << "- Level: " << currentLv << " / " << bData.maxLevel << "\n";

        // Nếu công trình đang trong tiến trình đếm ngược giây nâng cấp chạy ngầm
        auto buildQueueIt = facilityManager.activeConstructions.find(bId);
        if (buildQueueIt != facilityManager.activeConstructions.end()) {
            std::cout << "  >>> Upgrading " << buildQueueIt->second.targetLevel 
                      << " (Time left: " << std::fixed << std::setprecision(1) << buildQueueIt->second.timeRemaining << "s)\n\n";
            continue;
        }

        if (currentLv >= bData.maxLevel) {
            std::cout << "  + Max Level Reached!\n\n";
            continue;
        }

        int nextLv = currentLv + 1;
        auto lvConfigIt = bData.levels.find(nextLv);
        if (lvConfigIt != bData.levels.end()) {
            const LevelConfig& cfg = lvConfigIt->second;
            std::cout << "- Upgrade Requirement:\n";
            if (cfg.requirements.empty()) std::cout << "  + No requirements\n";
            for (const auto& req : cfg.requirements) {
                std::cout << "  + Tháp " << req.first << " Reach Level: " << req.second << "\n";
            }

            std::cout << "- Upgrade Cost:\n";
            for (const auto& cost : cfg.cost) {
                std::cout << "  + " << cost.first << ": " << cost.second << "\n";
            }
            std::cout << "  + Thời gian gốc: " << cfg.buildTime << "s\n";

            std::cout << "- Current & Next Bonus:\n";
            for (const auto& buff : cfg.buffs) {
                std::cout << "  + Hiệu ứng [" << buff.first << "]: " << buff.second << "\n";
            }
        }
        std::cout << "\n";
    }
    drawDivider();
    std::cout << "[0] Return (to Menu) | Để nâng cấp gõ: up <Mã_ID> (Ví dụ: up Academy)\n";
    drawDivider();
}

// Quản lý hiển thị Cây công nghệ nghiên cứu khoa học
void MenuRender::renderResearch(const Player& player, const DataParser& parser, const ResearchManager& researchManager) {
    drawDivider();
    std::cout << " PHÒNG THÍ NGHIỆM HỌC VIỆN (RESEARCH)\n";
    drawDivider();

    for (const auto& pair : parser.researchDatabase) {
        std::string rId = pair.first;
        const ResearchData& rData = pair.second;

        int currentLv = 0;
        auto pIt = player.researchLevels.find(rId);
        if (pIt != player.researchLevels.end()) currentLv = pIt->second;

        std::cout << rData.name << " [" << rId << "]\n";
        std::cout << "- Mô tả: " << rData.description << "\n";
        std::cout << "- Level: " << currentLv << "\n";

        auto resQueueIt = researchManager.activeResearches.find(rId);
        if (resQueueIt != researchManager.activeResearches.end()) {
            std::cout << "  >>> TRẠNG THÁI: Đang nghiên cứu cấp " << resQueueIt->second.targetLevel 
                      << " (Còn lại: " << std::fixed << std::setprecision(1) << resQueueIt->second.timeRemaining << "s)\n\n";
            continue;
        }

        int nextLv = currentLv + 1;
        auto lvConfigIt = rData.levels.find(nextLv);
        if (lvConfigIt != rData.levels.end()) {
            const LevelConfig& cfg = lvConfigIt->second;
            std::cout << "- Upgrade Requirement:\n";
            for (const auto& req : cfg.requirements) {
                std::cout << "  + Yêu cầu '" << req.first << "' đạt cấp: " << req.second << "\n";
            }

            std::cout << "- Upgrade Cost:\n";
            for (const auto& cost : cfg.cost) {
                std::cout << "  + " << cost.first << ": " << cost.second << "\n";
            }
            std::cout << "  + Thời gian gốc: " << cfg.buildTime << "s\n";

            std::cout << "- Current & Next Bonus:\n";
            for (const auto& buff : cfg.buffs) {
                std::cout << "  + Hiệu ứng [" << buff.first << "]: " << buff.second << "\n";
            }
        }
        std::cout << "\n";
    }
    drawDivider();
    std::cout << "[0] Return (to Menu) | Để nghiên cứu gõ: res <Mã_ID> (Ví dụ: res BasicLogistics)\n";
    drawDivider();
}