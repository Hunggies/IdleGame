#include "managers/DataParser.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

// --- HÀM TRỢ GIÚP (HELPERS) ---

// Xóa khoảng trắng thừa ở đầu và cuối chuỗi
std::string DataParser::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

// Phân tách chuỗi dạng "Key:Value, Key2:Value2" sang map<string, double>
std::map<std::string, double> DataParser::parseDoubleMap(const std::string& str) {
    std::map<std::string, double> result;
    if (str.empty()) return result;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, ',')) {
        size_t colon = token.find(':');
        if (colon != std::string::npos) {
            std::string key = trim(token.substr(0, colon));
            double value = std::stod(trim(token.substr(colon + 1)));
            result[key] = value;
        }
    }
    return result;
}

// Phân tách chuỗi dạng "Key:Value, Key2:Value2" sang map<string, long long>
std::map<std::string, long long> DataParser::parseLongMap(const std::string& str) {
    std::map<std::string, long long> result;
    if (str.empty()) return result;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, ',')) {
        size_t colon = token.find(':');
        if (colon != std::string::npos) {
            std::string key = trim(token.substr(0, colon));
            long long value = std::stoll(trim(token.substr(colon + 1)));
            result[key] = value;
        }
    }
    return result;
}

// Phân tách chuỗi dạng "Key:Value, Key2:Value2" sang map<string, int>
std::map<std::string, int> DataParser::parseIntMap(const std::string& str) {
    std::map<std::string, int> result;
    if (str.empty()) return result;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, ',')) {
        size_t colon = token.find(':');
        if (colon != std::string::npos) {
            std::string key = trim(token.substr(0, colon));
            int value = std::stoi(trim(token.substr(colon + 1)));
            result[key] = value;
        }
    }
    return result;
}

// Phân tách chuỗi dạng "Key:Value, Key2:Value2" sang map<string, string>
std::map<std::string, std::string> DataParser::parseStringMap(const std::string& str) {
    std::map<std::string, std::string> result;
    if (str.empty()) return result;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, ',')) {
        size_t colon = token.find(':');
        if (colon != std::string::npos) {
            std::string key = trim(token.substr(0, colon));
            std::string value = trim(token.substr(colon + 1));
            result[key] = value;
        }
    }
    return result;
}

// --- THỰC THI CHÍNH (MAIN PARSING LOGIC) ---

bool DataParser::loadAllData(const std::string& dataFolderPath) {
    bool success = true;
    success &= parseShopData(dataFolderPath + "/shop_data.txt");
    success &= parseBuildingData(dataFolderPath + "/building_data.txt");
    success &= parseResearchData(dataFolderPath + "/research_data.txt");
    success &= parseCharmData(dataFolderPath + "/charm_data.txt");
    return success;
}

bool DataParser::parseShopData(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "[Lỗi] Không thể mở file cấu hình Shop tại: " << filePath << std::endl;
        return false;
    }

    std::string line, currentCategory = "";
    ShopItemData currentItem;
    bool inItem = false;

    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue; // Bỏ qua dòng trống hoặc comment

        if (line.rfind("SHOP_CATEGORY:", 0) == 0) {
            currentCategory = trim(line.substr(14));
            shopDatabase[currentCategory].categoryId = currentCategory;
        } else if (line.rfind("ITEM_ID:", 0) == 0) {
            currentItem = ShopItemData();
            currentItem.id = trim(line.substr(8));
            inItem = true;
        } else if (inItem) {
            if (line.rfind("NAME:", 0) == 0) {
                currentItem.name = trim(line.substr(5));
            } else if (line.rfind("DESCRIPTION:", 0) == 0) {
                currentItem.description = trim(line.substr(12));
            } else if (line.rfind("PRICE:", 0) == 0) {
                currentItem.price = parseDoubleMap(line.substr(6));
            } else if (line.rfind("BUFF:", 0) == 0) {
                currentItem.buffs = parseDoubleMap(line.substr(5));
            } else if (line.rfind("EFFECT:", 0) == 0) {
                currentItem.effects = parseDoubleMap(line.substr(7));
            } else if (line == "END_ITEM") {
                if (!currentCategory.empty()) {
                    shopDatabase[currentCategory].items.push_back(currentItem);
                }
                inItem = false;
            }
        }
    }
    file.close();
    return true;
}

bool DataParser::parseBuildingData(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "[Lỗi] Không thể mở file cấu hình Building tại: " << filePath << std::endl;
        return false;
    }

    std::string line;
    BuildingData currentBuilding;
    LevelConfig currentLevel;
    bool inBuilding = false;
    int currentLvNum = 0;

    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;

        if (line.rfind("BUILDING_ID:", 0) == 0) {
            currentBuilding = BuildingData();
            currentBuilding.id = trim(line.substr(12));
            inBuilding = true;
        } else if (inBuilding) {
            if (line.rfind("NAME:", 0) == 0) {
                currentBuilding.name = trim(line.substr(5));
            } else if (line.rfind("MAX_LEVEL:", 0) == 0) {
                currentBuilding.maxLevel = std::stoi(trim(line.substr(10)));
            } else if (line.rfind("--LV:", 0) == 0 && line.size() >= 7) {
                // Đọc chỉ số cấp độ từ chuỗi "--LV:X--"
                size_t dash = line.find("--", 5);
                currentLvNum = std::stoi(trim(line.substr(5, dash - 5)));
                currentLevel = LevelConfig();
                currentLevel.level = currentLvNum;
            } else if (currentLvNum > 0) {
                if (line.rfind("COST:", 0) == 0) {
                    currentLevel.cost = parseLongMap(line.substr(5));
                } else if (line.rfind("TIME:", 0) == 0) {
                    currentLevel.buildTime = std::stod(trim(line.substr(5)));
                } else if (line.rfind("BUFF:", 0) == 0) {
                    currentLevel.buffs = parseStringMap(line.substr(5));
                    // Lưu cấu hình cấp độ vừa đọc xong vào Building
                    currentBuilding.levels[currentLvNum] = currentLevel;
                }
            }
            
            if (line == "END_BUILDING") {
                buildingDatabase[currentBuilding.id] = currentBuilding;
                inBuilding = false;
                currentLvNum = 0;
            }
        }
    }
    file.close();
    return true;
}

bool DataParser::parseResearchData(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "[Lỗi] Không thể mở file cấu hình Research tại: " << filePath << std::endl;
        return false;
    }

    std::string line;
    ResearchData currentResearch;
    LevelConfig currentLevel;
    bool inResearch = false;
    int currentLvNum = 0;

    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;

        if (line.rfind("RESEARCH_ID:", 0) == 0) {
            currentResearch = ResearchData();
            currentResearch.id = trim(line.substr(12));
            inResearch = true;
        } else if (inResearch) {
            if (line.rfind("NAME:", 0) == 0) {
                currentResearch.name = trim(line.substr(5));
            } else if (line.rfind("DESCRIPTION:", 0) == 0) {
                currentResearch.description = trim(line.substr(12));
            } else if (line.rfind("--LV:", 0) == 0 && line.size() >= 7) {
                size_t dash = line.find("--", 5);
                currentLvNum = std::stoi(trim(line.substr(5, dash - 5)));
                currentLevel = LevelConfig();
                currentLevel.level = currentLvNum;
            } else if (currentLvNum > 0) {
                if (line.rfind("REQUIRE:", 0) == 0) {
                    currentLevel.requirements = parseIntMap(line.substr(8));
                } else if (line.rfind("COST:", 0) == 0) {
                    currentLevel.cost = parseLongMap(line.substr(5));
                } else if (line.rfind("TIME:", 0) == 0) {
                    currentLevel.buildTime = std::stod(trim(line.substr(5)));
                } else if (line.rfind("BUFF:", 0) == 0) {
                    currentLevel.buffs = parseStringMap(line.substr(5));
                    currentResearch.levels[currentLvNum] = currentLevel;
                }
            }
            
            if (line == "END_RESEARCH") {
                researchDatabase[currentResearch.id] = currentResearch;
                inResearch = false;
                currentLvNum = 0;
            }
        }
    }
    file.close();
    return true;
}

bool DataParser::parseCharmData(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "[Lỗi] Không thể mở file cấu hình Charm tại: " << filePath << std::endl;
        return false;
    }

    std::string line;
    CharmData currentCharm;
    bool inCharm = false;

    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;

        if (line.rfind("CHARM_ID:", 0) == 0) {
            currentCharm = CharmData();
            currentCharm.id = trim(line.substr(9));
            inCharm = true;
        } else if (inCharm) {
            if (line.rfind("NAME:", 0) == 0) {
                currentCharm.name = trim(line.substr(5));
            } else if (line.rfind("DESCRIPTION:", 0) == 0) {
                currentCharm.description = trim(line.substr(12));
            } else if (line.rfind("BUFF_PER_LEVEL:", 0) == 0) {
                currentCharm.buffsPerLevel = parseDoubleMap(line.substr(15));
            } else if (line == "END_CHARM") {
                charmDatabase[currentCharm.id] = currentCharm;
                inCharm = false;
            }
        }
    }
    file.close();
    return true;
}