#ifndef RESEARCH_MANAGER_HPP
#define RESEARCH_MANAGER_HPP

#include "core/Player.hpp"
#include "core/StatManager.hpp"
#include "managers/DataParser.hpp"
#include <string>
#include <map>

// Cấu trúc lưu trạng thái công nghệ đang được nghiên cứu trong phòng thí nghiệm
struct ActiveResearch {
    std::string researchId;
    int targetLevel;
    double timeRemaining; // Tính bằng giây
};

class ResearchManager {
public:
    // Quản lý các công nghệ đang chạy tiến trình nghiên cứu
    std::map<std::string, ActiveResearch> activeResearches;

    // Hàm kích hoạt lệnh bắt đầu nghiên cứu công nghệ mới
    bool startResearch(const std::string& researchId, Player& player, const DataParser& parser, const CalculatedStats& stats, std::string& errorMsg);

    // Cập nhật tiến độ đếm ngược nghiên cứu thời gian thực
    void update(double deltaTime, Player& player);
};

#endif // RESEARCH_MANAGER_HPP