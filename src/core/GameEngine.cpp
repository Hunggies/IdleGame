#include "core/GameEngine.hpp"
#include "ui/ConsoleUtils.hpp"
#include <iostream>
#include <algorithm>

GameEngine::GameEngine() {
    currentState = UIState::HOME;
    selectedShopCategory = "";
    systemMessage = "";
    currentFishingBiome = "River";
    currentFarmingBiome = "GreenField";
    isRunning = true;
}

std::string GameEngine::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

bool GameEngine::initialize() {
    ConsoleUtils::setTextColor(36); // Cyan color for system logs
    std::cout << "=== WELCOME TO INCREMENTAL RPG ===\n";
    ConsoleUtils::resetColor();
    std::cout << "Enter your character name: ";
    std::getline(std::cin, player.name);
    if (player.name.empty()) player.name = "Unknown Hero";

    std::cout << "\n[System] Loading configuration database from 'data/' directory...\n";
    if (!parser.loadAllData("data") || !biomeMgr.loadBiomeData("data/biome_data.txt")) {
        ConsoleUtils::setTextColor(31); // Red color for errors
        std::cerr << "[Failure] Failed to load data files! Please check 'data/' directory.\n";
        ConsoleUtils::resetColor();
        return false;
    }
    std::cout << "[Success] All game data loaded successfully.\n";
    
    lastTickTime = std::chrono::steady_clock::now();
    return true;
}

void GameEngine::update(double deltaTime) {
    // Reduce action cooldowns
    if (player.cooldowns["FISHING"] > 0) player.cooldowns["FISHING"] -= deltaTime;
    if (player.cooldowns["FARMING"] > 0) player.cooldowns["FARMING"] -= deltaTime;
    if (player.cooldowns["FISHING"] < 0) player.cooldowns["FISHING"] = 0;
    if (player.cooldowns["FARMING"] < 0) player.cooldowns["FARMING"] = 0;

    // Update background tasks and resource generation
    CalculatedStats currentStats = statMgr.calculateStats(player, parser);
    facilityMgr.update(deltaTime, player, parser, currentStats);
    researchMgr.update(deltaTime, player);
}

void GameEngine::render() {
    ConsoleUtils::clearScreen();
    
    if (!systemMessage.empty()) {
        ConsoleUtils::setTextColor(33); // Yellow color for system notifications
        std::cout << "[NOTIFICATION]: " << systemMessage << "\n";
        ConsoleUtils::resetColor();
        systemMessage = "";
    }

    CalculatedStats currentStats = statMgr.calculateStats(player, parser);

    switch (currentState) {
        case UIState::HOME: renderer.renderHome(player, currentStats); break;
        case UIState::FISH_RESULT:
            if (!lastQueryResult.success && lastQueryResult.errorMsg.find("cooldown") != std::string::npos) {
                renderer.renderFishCooldown(player.cooldowns["FISHING"]);
            } else { renderer.renderFishUI(lastQueryResult); }
            break;
        case UIState::FARM_RESULT:
            if (!lastQueryResult.success && lastQueryResult.errorMsg.find("cooldown") != std::string::npos) {
                renderer.renderFarmCooldown(player.cooldowns["FARMING"]);
            } else { renderer.renderFarmUI(lastQueryResult); }
            break;
        case UIState::SHOP_MAIN: renderer.renderShopMain(); break;
        case UIState::SHOP_CATEGORY: renderer.renderShopCategory(selectedShopCategory, parser.shopDatabase[selectedShopCategory]); break;
        case UIState::MENU_MAIN: renderer.renderMenuMain(player); break;
        case UIState::QUESTS: renderer.renderQuests(); break;
        case UIState::CHARMS: renderer.renderCharms(player, parser); break;
        case UIState::BIOMES: renderer.renderBiomes(biomeMgr); break;
        case UIState::BUILD: renderer.renderBuild(player, parser, facilityMgr); break;
        case UIState::RESEARCH: renderer.renderResearch(player, parser, researchMgr); break;
    }
}

void GameEngine::handleInput(const std::string& rawInput) {
    std::string input = trim(rawInput);
    if (input == "exit" || input == "quit") {
        isRunning = false;
        std::cout << "Goodbye! Game process terminated.\n";
        return;
    }

    CalculatedStats currentStats = statMgr.calculateStats(player, parser);

    if (currentState == UIState::HOME) {
        if (input == "1") {
            lastQueryResult = biomeMgr.executeActivity(currentFishingBiome, player, currentStats);
            currentState = UIState::FISH_RESULT;
        } else if (input == "2") {
            lastQueryResult = biomeMgr.executeActivity(currentFarmingBiome, player, currentStats);
            currentState = UIState::FARM_RESULT;
        } else if (input == "3") { currentState = UIState::SHOP_MAIN; }
        else if (input == "4") { currentState = UIState::MENU_MAIN; }
    }
    else if (currentState == UIState::FISH_RESULT || currentState == UIState::FARM_RESULT) {
        if (input == "0") currentState = UIState::HOME;
        else if (input == "1") {
            std::string activeBiome = (currentState == UIState::FISH_RESULT) ? currentFishingBiome : currentFarmingBiome;
            lastQueryResult = biomeMgr.executeActivity(activeBiome, player, currentStats);
        }
    }
    else if (currentState == UIState::SHOP_MAIN) {
        if (input == "0") currentState = UIState::HOME;
        else if (input == "1") { currentState = UIState::SHOP_CATEGORY; selectedShopCategory = "RodShop"; }
        else if (input == "2") { currentState = UIState::SHOP_CATEGORY; selectedShopCategory = "HoeShop"; }
        else if (input == "3") { currentState = UIState::SHOP_CATEGORY; selectedShopCategory = "UpgradeShop"; }
        else if (input == "4") { currentState = UIState::SHOP_CATEGORY; selectedShopCategory = "SpecialShop"; }
    }
    else if (currentState == UIState::SHOP_CATEGORY) {
        if (input == "0") { currentState = UIState::SHOP_MAIN; }
        else {
            try {
                int idx = std::stoi(input);
                std::string errMsg;
                if (shopMgr.buyItem(idx, selectedShopCategory, player, parser, errMsg)) {
                    systemMessage = "Purchase successful!";
                } else { systemMessage = "Transaction failed: " + errMsg; }
            } catch (...) { systemMessage = "Please enter a valid item index number."; }
        }
    }
    else if (currentState == UIState::MENU_MAIN) {
        if (input == "0") currentState = UIState::HOME;
        else if (input == "1") currentState = UIState::QUESTS;
        else if (input == "2") currentState = UIState::CHARMS;
        else if (input == "3") currentState = UIState::BIOMES;
        else if (input == "4") currentState = UIState::BUILD;
        else if (input == "5") {
            if (player.facilityLevels["Academy"] > 0) currentState = UIState::RESEARCH;
            else systemMessage = "Please construct the Academy to unlock the Research system.";
        }
    }
    else if (currentState == UIState::QUESTS || currentState == UIState::CHARMS) {
        if (input == "0") currentState = UIState::MENU_MAIN;
    }
    else if (currentState == UIState::BIOMES) {
        if (input == "0") currentState = UIState::MENU_MAIN;
        else if (!input.empty() && biomeMgr.biomeDatabase.find(input) != biomeMgr.biomeDatabase.end()) {
            if (biomeMgr.biomeDatabase[input].type == "FISHING") {
                currentFishingBiome = input;
                systemMessage = "Fishing zone changed to: " + biomeMgr.biomeDatabase[input].name;
            } else {
                currentFarmingBiome = input;
                systemMessage = "Farming zone changed to: " + biomeMgr.biomeDatabase[input].name;
            }
            currentState = UIState::HOME;
        } else { systemMessage = "Invalid Biome ID!"; }
    }
    else if (currentState == UIState::BUILD) {
        if (input == "0") currentState = UIState::MENU_MAIN;
        else if (input.rfind("up ", 0) == 0) {
            std::string targetId = trim(input.substr(3));
            std::string errMsg;
            if (facilityMgr.startUpgrade(targetId, player, parser, currentStats, errMsg)) {
                systemMessage = "Facility upgrade added to background construction queue!";
            } else { systemMessage = "Upgrade failed: " + errMsg; }
        }
    }
    else if (currentState == UIState::RESEARCH) {
        if (input == "0") currentState = UIState::MENU_MAIN;
        else if (input.rfind("res ", 0) == 0) {
            std::string targetId = trim(input.substr(4));
            std::string errMsg;
            if (researchMgr.startResearch(targetId, player, parser, currentStats, errMsg)) {
                systemMessage = "Technology project added to background research queue!";
            } else { systemMessage = "Research failed: " + errMsg; }
        }
    }
}

void GameEngine::run() {
    while (isRunning) {
        auto currentTickTime = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed = currentTickTime - lastTickTime;
        double deltaTime = elapsed.count();
        lastTickTime = currentTickTime;

        update(deltaTime);
        render();

        std::cout << "Your choice: ";
        std::string input;
        std::getline(std::cin, input);
        handleInput(input);
    }
}