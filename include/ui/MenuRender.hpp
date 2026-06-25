#ifndef MENU_RENDER_HPP
#define MENU_RENDER_HPP

#include "core/Player.hpp"
#include "core/StatManager.hpp"
#include "managers/DataParser.hpp"
#include "managers/BiomeManager.hpp"
#include "managers/FacilityManager.hpp"
#include "managers/ResearchManager.hpp"

class MenuRender {
public:
    // Các hàm tiện ích màn hình console
    static void clearScreen();
    static void drawDivider();

    // [source 1] Giao diện màn hình chính (Home UI)
    void renderHome(const Player& player, const CalculatedStats& stats);

    // [source 3-6] Giao diện câu cá (Fish UI) & Trạng thái Cooldown
    void renderFishUI(const ActivityResult& result);
    void renderFishCooldown(double timeLeft);

    // [source 7-8] Giao diện nông trại (Farm UI) & Trạng thái Cooldown
    void renderFarmUI(const ActivityResult& result);
    void renderFarmCooldown(double timeLeft);

    // Giao diện Cửa hàng tổng và Danh mục hàng hóa
    void renderShopMain();
    void renderShopCategory(const std::string& categoryName, const ShopCategoryData& categoryData);

    // Màn hình Menu phụ chỉnh [source 4]
    void renderMenuMain(const Player& player);

    // Các phân hệ màn hình con trong Menu
    void renderQuests(); // Hệ thống nhiệm vụ (Quest)
    void renderCharms(const Player& player, const DataParser& parser); // Hệ thống bùa chú
    void renderBiomes(const BiomeManager& biomeManager); // Lựa chọn vùng đất sinh thái
    void renderBuild(const Player& player, const DataParser& parser, const FacilityManager& facilityManager); // Công trình
    void renderResearch(const Player& player, const DataParser& parser, const ResearchManager& researchManager); // Nghiên cứu
};

#endif // MENU_RENDER_HPP