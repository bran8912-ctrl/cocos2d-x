#include "AIEngine.h"
#include <algorithm>
#include <ctime>

AIEngine* AIEngine::s_instance = nullptr;

AIEngine* AIEngine::getInstance() {
    if (!s_instance) {
        s_instance = new AIEngine();
    }
    return s_instance;
}

AIEngine::AIEngine() : rng(std::time(nullptr)) {
    initializeData();
}

void AIEngine::initializeData() {
    firstNames = {"Alex", "Jordan", "Morgan", "Taylor", "Casey", "Riley", "Avery", "Quinn", 
                  "Sage", "River", "Phoenix", "Dakota", "Skyler", "Cameron", "Parker"};
    
    lastNames = {"Smith", "Johnson", "Williams", "Brown", "Jones", "Garcia", "Miller", 
                 "Davis", "Rodriguez", "Martinez", "Anderson", "Taylor", "Thomas"};
    
    traits = {"Friendly", "Professional", "Energetic", "Calm", "Ambitious", "Creative", 
              "Analytical", "Charismatic", "Reliable", "Innovative", "Patient", "Strategic"};
    
    eventTemplates = {
        "business_decision", "staff_issue", "guest_request", "expansion_opportunity",
        "competition", "reputation_event", "financial_challenge", "innovation_opportunity"
    };
}

std::string AIEngine::generateName() {
    std::uniform_int_distribution<> firstDist(0, firstNames.size() - 1);
    std::uniform_int_distribution<> lastDist(0, lastNames.size() - 1);
    return firstNames[firstDist(rng)] + " " + lastNames[lastDist(rng)];
}

Character AIEngine::generateCharacter(const std::string& role) {
    Character character;
    character.name = generateName();
    character.role = role;
    
    std::uniform_int_distribution<> statDist(50, 100);
    character.satisfaction = statDist(rng);
    character.loyalty = statDist(rng);
    
    // Assign 2-3 random traits
    std::uniform_int_distribution<> traitCountDist(2, 3);
    int traitCount = traitCountDist(rng);
    
    std::vector<std::string> availableTraits = traits;
    for (int i = 0; i < traitCount; i++) {
        std::uniform_int_distribution<> traitDist(0, availableTraits.size() - 1);
        int index = traitDist(rng);
        character.traits.push_back(availableTraits[index]);
        availableTraits.erase(availableTraits.begin() + index);
    }
    
    // Generate preferences
    character.preferences["luxury"] = statDist(rng);
    character.preferences["entertainment"] = statDist(rng);
    character.preferences["privacy"] = statDist(rng);
    character.preferences["social"] = statDist(rng);
    
    return character;
}

GameEvent AIEngine::generateEvent(int gameDay, int reputation) {
    GameEvent event;
    
    std::uniform_int_distribution<> eventDist(0, eventTemplates.size() - 1);
    std::string eventType = eventTemplates[eventDist(rng)];
    
    // Check player patterns to generate more relevant events
    if (playerChoiceHistory.count(eventType) > 0) {
        auto& history = playerChoiceHistory[eventType];
        // Adapt events based on player's previous choices
    }
    
    if (eventType == "business_decision") {
        event.title = "Expansion Opportunity";
        event.description = "A premium entertainment company wants to partner with your venue. This could bring in high-profile guests but requires significant investment.";
        event.choices = {
            "Accept partnership ($50,000)",
            "Negotiate better terms",
            "Decline politely"
        };
        event.impacts = {-50000, -25000, 0};
        
    } else if (eventType == "staff_issue") {
        event.title = "Staff Development";
        event.description = "Your head manager wants to implement a new training program for staff. It will cost money but could improve service quality.";
        event.choices = {
            "Invest in full program ($10,000)",
            "Basic training only ($3,000)",
            "Staff learns on the job"
        };
        event.impacts = {-10000, -3000, 0};
        
    } else if (eventType == "guest_request") {
        event.title = "VIP Guest Demand";
        event.description = "A celebrity guest wants exclusive access to your best facilities for a private event. High payment but may disappoint other guests.";
        event.choices = {
            "Grant exclusive access (+$30,000)",
            "Offer premium access (+$15,000)",
            "Treat like regular guest"
        };
        event.impacts = {30000, 15000, 0};
        
    } else if (eventType == "reputation_event") {
        event.title = "Media Attention";
        event.description = "A lifestyle magazine wants to feature your venue. This could boost your reputation significantly.";
        event.choices = {
            "Full feature with photo shoot",
            "Standard coverage",
            "Decline the feature"
        };
        event.impacts = {0, 0, 0};
    } else {
        event.title = "Business Challenge";
        event.description = "An unexpected situation requires your decision.";
        event.choices = {
            "Take decisive action",
            "Consider carefully",
            "Wait and see"
        };
        event.impacts = {0, 0, 0};
    }
    
    return event;
}

AIEngine::DecisionResult AIEngine::processDecision(const GameEvent& event, int choiceIndex) {
    DecisionResult result;
    
    if (choiceIndex >= 0 && choiceIndex < event.impacts.size()) {
        result.moneyChange = event.impacts[choiceIndex];
    } else {
        result.moneyChange = 0;
    }
    
    // Calculate reputation and satisfaction changes based on choice
    std::uniform_int_distribution<> repDist(-10, 30);
    std::uniform_int_distribution<> satDist(-5, 15);
    
    result.reputationChange = repDist(rng) + (choiceIndex * 5);
    result.satisfactionChange = satDist(rng);
    
    // Generate outcome description
    if (choiceIndex == 0) {
        result.outcome = "Your bold decision pays off! Guests are impressed with your commitment to excellence.";
    } else if (choiceIndex == 1) {
        result.outcome = "A balanced approach. The situation is handled professionally.";
    } else {
        result.outcome = "Playing it safe. The immediate impact is minimal.";
    }
    
    return result;
}

void AIEngine::recordPlayerChoice(const std::string& eventType, int choice) {
    playerChoiceHistory[eventType].push_back(choice);
}

std::map<std::string, std::vector<int>> AIEngine::getPlayerPatterns() {
    return playerChoiceHistory;
}
