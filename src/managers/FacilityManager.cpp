#include "managers/FacilityManager.hpp"
#include <cmath>
#include <sstream>

bool FacilityManager::startUpgrade(const std::string& buildingId, Player& player, const DataParser& parser, const CalculatedStats& stats, std::string& errorMsg) {
    // 1. Kiểm tra xem công trình này có đang được nâng cấp dở dang hay không
    if (activeConstructions.find(buildingId) != activeConstructions.end()) {
        errorMsg = "Công trình này đang trong quá trình nâng cấp!";
        return false;
    }

    // 2. Tìm dữ liệu gốc của công trình từ Database nạp từ file txt
    auto dbIt = parser.buildingDatabase.find(buildingId);
    if (dbIt == parser.buildingDatabase.end()) {
        errorMsg = "Không tìm thấy dữ liệu cấu hình công trình này!";
        return false;
    }
    const BuildingData& bData = dbIt->second;

    // 3. Tính toán cấp độ đích
    int currentLevel = 0;
    auto pIt = player.facilityLevels.find(buildingId);
    if (pIt != player.facilityLevels.end()) {
        currentLevel = pIt->second;
    }
    int targetLevel = currentLevel + 1;

    if (targetLevel > bData.maxLevel) {
        errorMsg = "Công trình đã đạt cấp độ tối đa (Cấp " + std::to_string(bData.maxLevel) + ")!";
        return false;
    }

    // 4. Tìm cấu hình chi phí và thời gian của Cấp độ đích
    auto lvIt = bData.levels.find(targetLevel);
    if (lvIt == bData.levels.end()) {
        errorMsg = "Không tìm thấy dữ liệu cấu hình cho cấp độ " + std::to_string(targetLevel) + "!";
        return false;
    }
    const LevelConfig& lvConfig = lvIt->second;

    // 5. Kiểm tra điều kiện tiên quyết (Nếu có quy định trong file dữ liệu)
    for (const auto& req : lvConfig.requirements) {
        // Nếu yêu cầu cấp độ của một công trình khác
        if (player.facilityLevels[req.first] < req.second) {
            errorMsg = "Không đủ điều kiện! Yêu cầu công trình " + req.first + " đạt cấp " + std::to_string(req.second);
            return false;
        }
    }

    // 6. Kiểm tra số dư tài nguyên Vật liệu (Material) có đủ để trả phí nâng cấp
    for (const auto& costItem : lvConfig.cost) {
        auto invIt = player.inventoryMaterial.find(costItem.first);
        if (invIt == player.inventoryMaterial.end() || invIt->second < costItem.second) {
            errorMsg = "Không đủ nguyên liệu nâng cấp! Thiếu: " + costItem.first;
            return false;
        }
    }

    // 7. Khấu trừ tài nguyên sau khi kiểm tra hợp lệ hoàn toàn
    for (const auto& costItem : lvConfig.cost) {
        player.deductResource("Material", costItem.first, costItem.second);
    }

    // 8. Tính toán thời gian xây dựng thực tế sau khi áp dụng chỉ số giảm tốc độ từ StatManager
    // Công thức: Thời gian thực = Thời gian gốc / (1.0 + Tỉ lệ cộng thêm tốc độ xây dựng)
    double finalBuildTime = lvConfig.buildTime / (1.0 + stats.buildingSpeedBonus);

    // 9. Đưa vào hàng đợi nâng cấp chạy ngầm thời gian thực
    ActiveConstruction construction;
    construction.buildingId = buildingId;
    construction.targetLevel = targetLevel;
    construction.timeRemaining = finalBuildTime;

    activeConstructions[buildingId] = construction;
    return true;
}

void FacilityManager::update(double deltaTime, Player& player, const DataParser& parser, const CalculatedStats& stats) {
    // I. LOGIC ĐẾM NGƯỢC THỜI GIAN XÂY DỰNG
    for (auto it = activeConstructions.begin(); it != activeConstructions.end();) {
        it->second.timeRemaining -= deltaTime;
        if (it->second.timeRemaining <= 0) {
            // Hoàn tất nâng cấp: Cập nhật cấp độ mới trực tiếp vào hồ sơ người chơi
            player.facilityLevels[it->second.buildingId] = it->second.targetLevel;
            it = activeConstructions.erase(it); // Xóa khỏi danh sách đếm ngược
        } else {
            ++it;
        }
    }

    // II. LOGIC SẢN XUẤT TÀI NGUYÊN THỤ ĐỘNG (PASSIVE GENERATION)
    // Duyệt qua tất cả các công trình mà người chơi đang sở hữu cấp độ > 0
    for (const auto& pair : player.facilityLevels) {
        std::string bId = pair.first;
        int bLevel = pair.second;
        if (bLevel <= 0) continue;

        // Tìm cấu hình bùa lợi sản lượng của công trình tại cấp độ hiện tại
        auto dbIt = parser.buildingDatabase.find(bId);
        if (dbIt == parser.buildingDatabase.end()) continue;

        auto lvIt = dbIt->second.levels.find(bLevel);
        if (lvIt == dbIt->second.levels.end()) continue;

        const LevelConfig& lvConfig = lvIt->second;

        // Quét tìm từ khóa "PassiveGeneration" trong map buff chuỗi
        auto buffIt = lvConfig.buffs.find("PassiveGeneration");
        if (buffIt != lvConfig.buffs.end()) {
            // Chuỗi định dạng từ file text có dạng: "Food:10" (Tên tài nguyên : Sản lượng mỗi giờ)
            std::string rawValue = buffIt->second;
            size_t colon = rawValue.find(':');
            if (colon != std::string::npos) {
                std::string resId = rawValue.substr(0, colon);
                double amountPerHour = std::stod(rawValue.substr(colon + 1));

                // Quy đổi sản lượng: Mỗi giây đẻ ra = (Sản lượng mỗi giờ / 3600 giây)
                double amountPerSecond = amountPerHour / 3600.0;
                
                // Áp dụng bùa chú tăng tốc độ đẻ thụ động từ ProducingCharm của StatManager
                double finalProduction = amountPerSecond * deltaTime * stats.passiveGenSpeedMultiplier;

                // Tích lũy vào bộ nhớ đệm số lẻ float
                passiveResourceAccumulators[resId] += finalProduction;

                // Nếu tích lũy đủ lớn hơn hoặc bằng 1 đơn vị nguyên, tiến hành chuyển giao vào ví Player
                if (passiveResourceAccumulators[resId] >= 1.0) {
                    long long integerPart = static_cast<long long>(std::floor(passiveResourceAccumulators[resId]));
                    player.addResource("Material", resId, integerPart);
                    passiveResourceAccumulators[resId] -= integerPart; // Giữ lại phần thập phân thừa
                }
            }
        }
    }
}