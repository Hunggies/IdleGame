#ifndef DATA_PARSER_HPP
#define DATA_PARSER_HPP

#include <string>
#include <vector>
#include <map>

// --- Cấu trúc dữ liệu cho Shop ---
struct ShopItemData {
    std::string id;
    std::string name;
    std::string description;
    std::map<std::string, double> price;       // Ví dụ: {"Money": 5000, "Wood": 200}
    std::map<std::string, double> buffs;       // Ví dụ: {"FishAmountMultiplier": 1.15}
    std::map<std::string, double> effects;     // Ví dụ: {"ReduceTime": 900}
};

struct ShopCategoryData {
    std::string categoryId;
    std::vector<ShopItemData> items;
};

// --- Cấu trúc dữ liệu cho Cấp độ (Dùng cho Building & Research) ---
struct LevelConfig {
    int level;
    std::map<std::string, int> requirements;   // Điều kiện mở khóa. Ví dụ: {"Academy": 2, "BasicLogistics": 5}
    std::map<std::string, long long> cost;     // Chi phí nâng cấp. Ví dụ: {"Wood": 100, "Stone": 50}
    double buildTime;                          // Thời gian xây/nghiên cứu (giây)
    std::map<std::string, std::string> buffs;  // Hiệu ứng bùa lợi. Ví dụ: {"MaxFacilityLevel": "1", "GatheringSpeed": "0.05"}
};

// --- Cấu trúc dữ liệu cho Cơ sở vật chất (Building) ---
struct BuildingData {
    std::string id;
    std::string name;
    int maxLevel;
    std::map<int, LevelConfig> levels;         // Key là số cấp (1 -> 25)
};

// --- Cấu trúc dữ liệu cho Nghiên cứu (Research) ---
struct ResearchData {
    std::string id;
    std::string name;
    std::string description;
    std::map<int, LevelConfig> levels;         // Key là số cấp
};

// --- Cấu trúc dữ liệu cho Linh vật / Bùa chú (Charm) ---
struct CharmData {
    std::string id;
    std::string name;
    std::string description;
    std::map<std::string, double> buffsPerLevel; // Hệ số cộng thêm mỗi cấp. Ví dụ: {"SellPriceMultiplier": 0.05}
};

// --- Lớp Parser chính ---
class DataParser {
public:
    // Các kho lưu trữ dữ liệu tổng sau khi nạp thành công
    std::map<std::string, ShopCategoryData> shopDatabase;
    std::map<std::string, BuildingData> buildingDatabase;
    std::map<std::string, ResearchData> researchDatabase;
    std::map<std::string, CharmData> charmDatabase;

    // Hàm thực thi nạp toàn bộ dữ liệu từ thư mục
    bool loadAllData(const std::string& dataFolderPath);

private:
    // Các hàm parse từng file riêng biệt
    bool parseShopData(const std::string& filePath);
    bool parseBuildingData(const std::string& filePath);
    bool parseResearchData(const std::string& filePath);
    bool parseCharmData(const std::string& filePath);

    // Các hàm trợ giúp bóc tách chuỗi (Helper Functions)
    static std::string trim(const std::string& str);
    static std::map<std::string, double> parseDoubleMap(const std::string& str);
    static std::map<std::string, long long> parseLongMap(const std::string& str);
    static std::map<std::string, int> parseIntMap(const std::string& str);
    static std::map<std::string, std::string> parseStringMap(const std::string& str);
};

#endif // DATA_PARSER_HPP