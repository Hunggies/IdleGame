#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <map>
#include <vector>

// Cấu trúc lưu trữ các hiệu ứng buff tạm thời (Boost Shop)
struct ActiveEffect {
    std::string id;
    std::string name;
    double durationLeft; // Tính bằng giây
    double value;
};

class Player {
public:
    // Thông tin cơ bản (Profile)
    std::string name;
    int level;
    long long exp;
    double balance;
    
    // Hệ thống Tiến trình (Progression)
    int rebirthCount;
    int prestigeLevel;
    int prestigeTokens;

    // Kho hành lý phân loại theo thiết kế (Inventory)
    std::map<std::string, long long> inventoryBasic;    // Cá thường, nông sản thường
    std::map<std::string, long long> inventorySpecial;  // Cá hiếm, nông sản hiếm (Golden Fish,...)
    std::map<std::string, long long> inventoryMaterial; // 6 loại nguyên liệu chính (Food, Wood, Stone,...)

    // Cấp độ của các thực thể sở hữu (Đọc từ file dữ liệu)
    std::map<std::string, int> charmLevels;
    std::map<std::string, int> facilityLevels;
    std::map<std::string, int> researchLevels;

    // Danh sách hiệu ứng đang hoạt động
    std::vector<ActiveEffect> activeEffects;

    // Quản lý thời gian hồi chiêu (Cooldowns) tính bằng giây
    std::map<std::string, double> cooldowns;

    // Khởi tạo mặc định cho người chơi mới
    Player();

    // Các hàm bổ trợ thay đổi dữ liệu an toàn
    void addMoney(double amount);
    bool deductMoney(double amount);
    void addEXP(long long amount);
    
    // Tương tác túi đồ theo phân loại
    void addResource(const std::string& category, const std::string& id, long long amount);
    bool deductResource(const std::string& category, const std::string& id, long long amount);

    // Hàm tính toán giới hạn dựa trên logic thiết kế
    long long getRequiredEXP() const;
    int getMaxLevel() const;

    // Cập nhật thời gian thực (Thread ngầm sẽ gọi hàm này)
    void updateTimers(double deltaTime);

    long long getNextLevelEXP() const {
        return level * 1000;
    }
};

#endif // PLAYER_HPP