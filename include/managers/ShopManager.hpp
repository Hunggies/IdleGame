#ifndef SHOP_MANAGER_HPP
#define SHOP_MANAGER_HPP

#include "core/Player.hpp"
#include "managers/DataParser.hpp"
#include <string>

class ShopManager {
public:
    // Xử lý logic mua hàng bằng số thứ tự chọn trên màn hình UI Console
    bool buyItem(int itemIndex, const std::string& categoryId, Player& player, const DataParser& parser, std::string& errorMsg);
};

#endif // SHOP_MANAGER_HPP