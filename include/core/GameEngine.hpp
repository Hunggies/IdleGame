#ifndef GAME_ENGINE_HPP
#define GAME_ENGINE_HPP

#include "core/Player.hpp"
#include "core/StatManager.hpp"
#include "managers/DataParser.hpp"
#include "managers/BiomeManager.hpp"
#include "managers/FacilityManager.hpp"
#include "managers/ResearchManager.hpp"
#include "managers/ShopManager.hpp"
#include "ui/MenuRender.hpp"
#include <string>
#include <chrono>

enum class UIState {
    HOME, FISH_RESULT, FARM_RESULT, SHOP_MAIN, SHOP_CATEGORY,
    MENU_MAIN, QUESTS, CHARMS, BIOMES, BUILD, RESEARCH
};

class GameEngine {
private:
    Player player;
    StatManager statMgr;
    DataParser parser;
    BiomeManager biomeMgr;
    FacilityManager facilityMgr;
    ResearchManager researchMgr;
    ShopManager shopMgr;
    MenuRender renderer;

    UIState currentState;
    std::string selectedShopCategory;
    std::string systemMessage;
    std::string currentFishingBiome;
    std::string currentFarmingBiome;
    bool isRunning;
    
    ActivityResult lastQueryResult;
    std::chrono::steady_clock::time_point lastTickTime;

    void handleInput(const std::string& input);
    void update(double deltaTime);
    void render();
    std::string trim(const std::string& str);

public:
    GameEngine();
    bool initialize();
    void run();
};

#endif // GAME_ENGINE_HPP