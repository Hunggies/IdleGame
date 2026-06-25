#include "managers/ShopManager.hpp"

bool ShopManager::buyItem(int itemIndex, const std::string& categoryId, Player& player, const DataParser& parser, std::string& errorMsg) {
    // 1. Kiểm tra danh mục cửa hàng có tồn tại không
    auto catIt = parser.shopDatabase.find(categoryId);
    if (catIt == parser.shopDatabase.end()) {
        errorMsg = "Không tìm thấy phân khu cửa hàng này!";
        return false;
    }
    const ShopCategoryData& category = catIt->second;

    // 2. Kiểm tra số thứ tự người chơi nhập vào có hợp lệ không
    if (itemIndex < 1 || itemIndex > static_cast<int>(category.items.size())) {
        errorMsg = "Mã số vật phẩm không tồn tại trong danh sách bày bán!";
        return false;
    }

    const ShopItemData& item = category.items[itemIndex - 1];

    // 3. Kiểm tra số dư tài khoản tiền mặt (balance) và nguyên liệu trao đổi
    for (const auto& cost : item.price) {
        if (cost.first == "Money" || cost.first == "money" || cost.first == "Balance") {
            if (player.balance < cost.second) {
                errorMsg = "Số dư tài khoản không đủ để thanh toán!";
                return false;
            }
        } else {
            auto invIt = player.inventoryMaterial.find(cost.first);
            if (invIt == player.inventoryMaterial.end() || invIt->second < cost.second) {
                errorMsg = "Không đủ nguyên liệu trao đổi! Thiếu: " + cost.first;
                return false;
            }
        }
    }

    // 4. Khấu trừ chi phí giao dịch sau khi kiểm tra hợp lệ
    for (const auto& cost : item.price) {
        if (cost.first == "Money" || cost.first == "money" || cost.first == "Balance") {
            player.balance -= cost.second;
        } else {
            player.deductResource("Material", cost.first, cost.second);
        }
    }

    // 5. Cấp phát vật phẩm/công cụ mới vào kho lưu trữ của người chơi
    if (categoryId == "RodShop" || categoryId == "HoeShop") {
        player.inventorySpecial[item.id] += 1; // Tăng cấp độ hoặc số lượng công cụ đặc biệt
    } else {
        player.inventoryMaterial[item.id] += 1; // Thêm vào túi đồ cơ bản/nguyên liệu
    }

    return true;
}