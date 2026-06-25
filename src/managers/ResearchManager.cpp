#include "managers/ResearchManager.hpp"
#include <cmath>

bool ResearchManager::startResearch(const std::string& researchId, Player& player, const DataParser& parser, const CalculatedStats& stats, std::string& errorMsg) {
    // 1. Điều kiện tiên quyết tối cao từ GDD giao diện: Phải xây dựng nhà Học viện (Academy) mới được mở khóa Research
    if (player.facilityLevels["Academy"] <= 0) {
        errorMsg = "Hệ thống Nghiên cứu đang khóa! Bạn cần xây dựng Học viện (Academy) trước.";
        return false;
    }

    // 2. Kiểm tra xem công nghệ này có đang trong phòng thí nghiệm nghiên cứu hay chưa
    if (activeResearches.find(researchId) != activeResearches.end()) {
        errorMsg = "Công nghệ này đang được tiến hành nghiên cứu!";
        return false;
    }

    // 3. Tìm dữ liệu gốc của công nghệ từ Database
    auto dbIt = parser.researchDatabase.find(researchId);
    if (dbIt == parser.researchDatabase.end()) {
        errorMsg = "Không tìm thấy dữ liệu cấu hình nghiên cứu này!";
        return false;
    }
    const ResearchData& rData = dbIt->second;

    // 4. Tính toán cấp độ đích tiếp theo
    int currentLevel = 0;
    auto pIt = player.researchLevels.find(researchId);
    if (pIt != player.researchLevels.end()) {
        currentLevel = pIt->second;
    }
    int targetLevel = currentLevel + 1;

    // Tìm cấu hình dữ liệu của cấp độ đích
    auto lvIt = rData.levels.find(targetLevel);
    if (lvIt == rData.levels.end()) {
        errorMsg = "Công nghệ này đã đạt cấp tối đa hoặc không có cấu hình cấp tiếp theo!";
        return false;
    }
    const LevelConfig& lvConfig = lvIt->second;

    // 5. Kiểm tra điều kiện chéo phức tạp (Requirements): Yêu cầu cấp độ của công nghệ khác HOẶC công trình khác
    for (const auto& req : lvConfig.requirements) {
        std::string reqId = req.first;
        int requiredLevel = req.second;

        // Tiến hành check xem mã reqId này là một công trình hay là một nghiên cứu khác
        bool criteriaMet = false;
        
        // Check trong danh sách công trình trước
        if (player.facilityLevels.find(reqId) != player.facilityLevels.end()) {
            if (player.facilityLevels[reqId] >= requiredLevel) criteriaMet = true;
        }
        // Sau đó check trong danh sách nghiên cứu
        else if (player.researchLevels.find(reqId) != player.researchLevels.end()) {
            if (player.researchLevels[reqId] >= requiredLevel) criteriaMet = true;
        }
        // Trường hợp người chơi chưa từng có tí cấp độ nào của thực thể yêu cầu đó
        else {
            if (requiredLevel <= 0) criteriaMet = true;
        }

        if (!criteriaMet) {
            errorMsg = "Không đủ điều kiện! Yêu cầu '" + reqId + "' đạt tối thiểu cấp " + std::to_string(requiredLevel);
            return false;
        }
    }

    // 6. Kiểm tra số dư tài nguyên nâng cấp
    for (const auto& costItem : lvConfig.cost) {
        auto invIt = player.inventoryMaterial.find(costItem.first);
        if (invIt == player.inventoryMaterial.end() || invIt->second < costItem.second) {
            errorMsg = "Không đủ tài nguyên nghiên cứu! Thiếu: " + costItem.first;
            return false;
        }
    }

    // 7. Khấu trừ chi phí tài nguyên hợp lệ
    for (const auto& costItem : lvConfig.cost) {
        player.deductResource("Material", costItem.first, costItem.second);
    }

    // 8. Tính toán thời gian nghiên cứu thực tế sau khi áp dụng chỉ số giảm tốc độ nghiên cứu từ StatManager
    double finalResearchTime = lvConfig.buildTime / (1.0 + stats.researchSpeedBonus);

    // 9. Đưa vào hàng đợi tiến trình nghiên cứu chạy ngầm
    ActiveResearch research;
    research.researchId = researchId;
    research.targetLevel = targetLevel;
    research.timeRemaining = finalResearchTime;

    activeResearches[researchId] = research;
    return true;
}

void ResearchManager::update(double deltaTime, Player& player) {
    // LOGIC ĐẾM NGƯỢC THỜI GIAN NGHIÊN CỨU TRONG PHÒNG THÍ NGHIỆM
    for (auto it = activeResearches.begin(); it != activeResearches.end();) {
        it->second.timeRemaining -= deltaTime;
        if (it->second.timeRemaining <= 0) {
            // Nghiên cứu thành công: Nâng cấp trực tiếp cây công nghệ cho người chơi
            player.researchLevels[it->second.researchId] = it->second.targetLevel;
            it = activeResearches.erase(it); // Giải phóng phòng thí nghiệm cho dự án tiếp theo
        } else {
            ++it;
        }
    }
}