#ifndef BIOME_MANAGER_HPP
#define BIOME_MANAGER_HPP

#include "core/Player.hpp"
#include "core/StatManager.hpp"
#include <string>
#include <vector>
#include <map>

// Cấu trúc đại diện cho một vật phẩm sinh sống tại Biome
struct BiomeItem {
    std::string id;        // Tên định danh (Ví dụ: Cod, Salmon, Wheat...)
    double baseChance;     // Tỉ lệ xuất hiện gốc (Ví dụ: 0.50 tương đương 50%)
    int rarityTier;        // Cấp độ hiếm: 0 là Thường (Common), 1 là Hiếm (Rare), 2 là Cực hiếm (Epic)
    long long expValue;    // Lượng EXP người chơi nhận được trên mỗi đơn vị câu trúng
};

// Cấu trúc chứa toàn bộ thông số vùng đất nạp từ file cấu hình ngoài
struct BiomeData {
    std::string id;
    std::string name;
    std::string type;       // Định loại hoạt động: "FISHING" hoặc "FARMING"
    int levelReq;           // Cấp độ người chơi tối thiểu để mở khóa vùng đất
    double baseAmount;      // Chỉ số Flat Amount gốc của Biome để tính sản lượng
    double baseInterval;    // Thời gian giãn cách hồi chiêu mặc định (tính bằng giây)
    std::vector<BiomeItem> lootTable; // Danh sách hệ sinh thái vật phẩm bên trong
};

// Cấu trúc đóng gói toàn bộ kết quả trả ra sau một cú Click Fish hoặc Farm của người chơi
struct ActivityResult {
    std::string biomeName;
    bool success;
    std::string errorMsg;   // Chứa chuỗi thông báo nếu bị vướng Cooldown hoặc thiếu Cấp
    
    // Phần thưởng thu hoạch chính
    std::map<std::string, long long> gatheredItems;
    long long totalExpGained;

    // Phần thưởng từ Rương báu kho báu (nếu may mắn nổ trúng)
    bool foundChest;
    std::string chestRarity; // Wood, Iron, Gold, Diamond
    long long chestExp;
    double chestMoney;
    std::map<std::string, long long> chestSpecialResources;
    std::map<std::string, int> chestCharms;
};

class BiomeManager {
public:
    // Kho dữ liệu lưu trữ toàn bộ các Biome sau khi quét file cấu hình thành công
    std::map<std::string, BiomeData> biomeDatabase;

    // Hàm đọc file cấu hình biome_data.txt ngoài đời thực
    bool loadBiomeData(const std::string& filePath);

    // Hàm xử lý Logic Gameplay: Thực hiện câu cá hoặc cuốc đất thời gian thực
    ActivityResult executeActivity(const std::string& biomeId, Player& player, const CalculatedStats& stats);
};

#endif // BIOME_MANAGER_HPP