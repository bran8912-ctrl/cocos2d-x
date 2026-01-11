#include "GameManager.h"

GameManager* GameManager::s_instance = nullptr;

GameManager* GameManager::getInstance() {
    if (!s_instance) {
        s_instance = new GameManager();
    }
    return s_instance;
}

GameManager::GameManager() 
    : money(100000)
    , reputation(50)
    , currentDay(1)
    , hasActiveEvent(false)
{
}

void GameManager::startNewGame() {
    reset();
    
    // Hire initial staff
    for (int i = 0; i < 5; i++) {
        hireStaff();
    }
    
    // Welcome initial guests
    for (int i = 0; i < 3; i++) {
        welcomeGuest();
    }
    
    generateNewEvent();
}

void GameManager::reset() {
    money = 100000;
    reputation = 50;
    currentDay = 1;
    staff.clear();
    guests.clear();
    hasActiveEvent = false;
}

void GameManager::hireStaff() {
    auto newStaff = AIEngine::getInstance()->generateCharacter("staff");
    staff.push_back(newStaff);
}

void GameManager::welcomeGuest() {
    auto newGuest = AIEngine::getInstance()->generateCharacter("guest");
    guests.push_back(newGuest);
}

void GameManager::updateCharacters() {
    // Update satisfaction based on game state
    for (auto& character : staff) {
        if (money < 50000) {
            character.satisfaction -= 5;
        }
        if (reputation > 75) {
            character.satisfaction += 3;
        }
    }
    
    for (auto& character : guests) {
        if (reputation > 70) {
            character.satisfaction += 5;
        }
    }
    
    // Remove unsatisfied guests
    guests.erase(
        std::remove_if(guests.begin(), guests.end(), 
            [](const Character& c) { return c.satisfaction < 20; }),
        guests.end()
    );
}

void GameManager::generateNewEvent() {
    currentEvent = AIEngine::getInstance()->generateEvent(currentDay, reputation);
    hasActiveEvent = true;
}

void GameManager::handleDecision(int choiceIndex, std::function<void(const AIEngine::DecisionResult&)> callback) {
    if (!hasActiveEvent) return;
    
    auto result = AIEngine::getInstance()->processDecision(currentEvent, choiceIndex);
    
    // Record the choice for learning
    AIEngine::getInstance()->recordPlayerChoice("event", choiceIndex);
    
    // Apply results
    addMoney(result.moneyChange);
    addReputation(result.reputationChange);
    
    // Update character satisfaction
    for (auto& character : staff) {
        character.satisfaction += result.satisfactionChange / 2;
    }
    for (auto& character : guests) {
        character.satisfaction += result.satisfactionChange;
    }
    
    hasActiveEvent = false;
    
    if (callback) {
        callback(result);
    }
}
