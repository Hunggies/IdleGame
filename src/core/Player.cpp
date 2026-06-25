#include "core/Player.hpp"
#include <cmath>
#include <algorithm>

Player::Player() {
    name = "Player";
    level = 0;
    exp = 0;
    balance = 0.0;
    rebirthCount = 0;
    prestigeLevel = 0;
    prestigeTokens = 0;

    // Khởi tạo mặc định các tài nguyên cơ bản bằng 0
    inventoryMaterial["Food"] = 0;
    inventoryMaterial["Wood"] = 0;
    inventoryMaterial["Stone"] = 0;
    inventoryMaterial["Coin"] = 0;
    inventoryMaterial["Gem"] = 0;
    inventoryMaterial["Crystal"] = 0;

    // Khởi tạo cấp độ công trình ban đầu (Tòa thị chính cấp 1, các công trình khác cấp 0)
    facilityLevels["TownHall"] = 1;
}

void Player::addMoney(double amount) {
    if (amount > 0) {
        balance += amount;
    }
}

bool Player::deductMoney(double amount) {
    if (amount <= balance) {
        balance -= amount;
        return true;
    }
    return false;
}

void Player::addEXP(long long amount) {
    if (amount <= 0) return;
    
    exp += amount;
    int maxLevel = getMaxLevel();

    // Vòng lặp kiểm tra lên cấp liên tục nếu đủ EXP
    while (level < maxLevel && exp >= getRequiredEXP()) {
        exp -= getRequiredEXP();
        level++;
    }
}

void Player::addResource(const std::string& category, const std::string& id, long long amount) {
    if (amount <= 0) return;
    
    if (category == "Basic") {
        inventoryBasic[id] += amount;
    } else if (category == "Special") {
        inventorySpecial[id] += amount;
    } else if (category == "Material") {
        inventoryMaterial[id] += amount;
    }
}

bool Player::deductResource(const std::string& category, const std::string& id, long long amount) {
    if (amount <= 0) return true;

    if (category == "Basic" && inventoryBasic[id] >= amount) {
        inventoryBasic[id] -= amount;
        return true;
    } else if (category == "Special" && inventorySpecial[id] >= amount) {
        inventorySpecial[id] -= amount;
        return true;
    } else if (category == "Material" && inventoryMaterial[id] >= amount) {
        inventoryMaterial[id] -= amount;
        return true;
    }
    return false;
}

long long Player::getRequiredEXP() const {
    // Công thức: Base EXP = level^3 * 1.15^PrestigeLevel
    double baseExp = std::pow(level, 3);
    double prestigeMultiplier = std::pow(1.15, prestigeLevel);
    return static_cast<long long>(baseExp * prestigeMultiplier);
}

int Player::getMaxLevel() const {
    // Khởi đầu 250, tăng 50 mỗi lần Rebirth, tối đa 1500
    int calculatedMax = 250 + (rebirthCount * 50);
    return std::min(calculatedMax, 1500);
}

void Player::updateTimers(double deltaTime) {
    // 1. Cập nhật Cooldowns của các hành động bấm tay (Fish/Farm)
    for (auto& pair : cooldowns) {
        if (pair.second > 0) {
            pair.second -= deltaTime;
            if (pair.second < 0) pair.second = 0;
        }
    }

    // 2. Cập nhật thời gian còn lại của các hiệu ứng Buff (Boost)
    for (auto it = activeEffects.begin(); it != activeEffects.end();) {
        it->durationLeft -= deltaTime;
        if (it->durationLeft <= 0) {
            it = activeEffects.erase(it); // Xóa hiệu ứng khi hết thời gian
        } else {
            ++it;
        }
    }
}