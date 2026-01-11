#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include "AIEngine.h"
#include <vector>
#include <functional>

class GameManager {
public:
    static GameManager* getInstance();
    
    // Game state
    int getMoney() const { return money; }
    int getReputation() const { return reputation; }
    int getDay() const { return currentDay; }
    
    void addMoney(int amount) { money += amount; }
    void addReputation(int amount) { reputation += amount; }
    void nextDay() { currentDay++; }
    
    // Character management
    std::vector<Character>& getStaff() { return staff; }
    std::vector<Character>& getGuests() { return guests; }
    
    void hireStaff();
    void welcomeGuest();
    void updateCharacters();
    
    // Event handling
    GameEvent getCurrentEvent() const { return currentEvent; }
    void generateNewEvent();
    void handleDecision(int choiceIndex, std::function<void(const AIEngine::DecisionResult&)> callback);
    
    // Game initialization
    void startNewGame();
    void reset();
    
private:
    GameManager();
    static GameManager* s_instance;
    
    int money;
    int reputation;
    int currentDay;
    
    std::vector<Character> staff;
    std::vector<Character> guests;
    
    GameEvent currentEvent;
    bool hasActiveEvent;
};

#endif // __GAME_MANAGER_H__
