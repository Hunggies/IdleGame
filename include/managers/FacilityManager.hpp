#ifndef FACILITY_MANAGER_HPP
#define FACILITY_MANAGER_HPP

#include "core/Player.hpp"
#include "core/StatManager.hpp"
#include "managers/DataParser.hpp"
#include <string>
#include <map>

// Cấu trúc lưu trạng thái công trình đang xây dựng/nâng cấp
struct ActiveConstruction {
    std::string buildingId;
    int targetLevel;
    double timeRemaining; // Tính bằng giây
};

class FacilityManager {
public:
    // Lưu danh sách các công trình đang trong tiến trình nâng cấp
    std::map<std::string, ActiveConstruction> activeConstructions;
    
    // Bộ tích lũy tài nguyên lẻ (Tránh mất mát số thập phân khi cộng dồn sản lượng thụ động mỗi giây)
    std::map<std::string, double> passiveResourceAccumulators;

    // Hàm kích hoạt lệnh nâng cấp công trình
    bool startUpgrade(const std::string& buildingId, Player& player, const DataParser& parser, const CalculatedStats& stats, std::string& errorMsg);
    
    // Cập nhật thời gian thực: Đếm ngược công trình và Tự động đẻ tài nguyên
    void update(double deltaTime, Player& player, const DataParser& parser, const CalculatedStats& stats);
};

#endif // FACILITY_MANAGER_HPP