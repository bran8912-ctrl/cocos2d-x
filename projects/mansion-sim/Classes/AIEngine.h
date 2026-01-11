#ifndef __AI_ENGINE_H__
#define __AI_ENGINE_H__

#include <string>
#include <vector>
#include <random>
#include <map>

struct Character {
    std::string name;
    std::string role;        // staff, guest, vip
    int satisfaction;
    int loyalty;
    std::vector<std::string> traits;
    std::map<std::string, int> preferences;
};

struct GameEvent {
    std::string title;
    std::string description;
    std::vector<std::string> choices;
    std::vector<int> impacts;  // Impact on money, reputation, etc.
};

class AIEngine {
public:
    static AIEngine* getInstance();
    
    // Character generation
    Character generateCharacter(const std::string& role);
    
    // Event generation
    GameEvent generateEvent(int gameDay, int reputation);
    
    // Decision processing
    struct DecisionResult {
        int moneyChange;
        int reputationChange;
        int satisfactionChange;
        std::string outcome;
    };
    
    DecisionResult processDecision(const GameEvent& event, int choiceIndex);
    
    // Learning system - tracks player choices to generate more relevant events
    void recordPlayerChoice(const std::string& eventType, int choice);
    std::map<std::string, std::vector<int>> getPlayerPatterns();
    
private:
    AIEngine();
    static AIEngine* s_instance;
    
    std::mt19937 rng;
    std::map<std::string, std::vector<int>> playerChoiceHistory;
    
    std::vector<std::string> firstNames;
    std::vector<std::string> lastNames;
    std::vector<std::string> traits;
    std::vector<std::string> eventTemplates;
    
    void initializeData();
    std::string generateName();
};

#endif // __AI_ENGINE_H__
