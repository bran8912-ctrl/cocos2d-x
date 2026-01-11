#ifndef __SAVE_MANAGER_H__
#define __SAVE_MANAGER_H__

#include "cocos2d.h"
#include "GameManager.h"
#include <string>

// Save/Load system inspired by Cocos2d-x UserDefault patterns
class SaveManager {
public:
    static SaveManager* getInstance();
    
    // Save game state
    void saveGame(const std::string& slotName = "quicksave");
    
    // Load game state
    bool loadGame(const std::string& slotName = "quicksave");
    
    // Check if save exists
    bool hasSaveData(const std::string& slotName = "quicksave");
    
    // Auto-save functionality
    void enableAutoSave(bool enable, float interval = 300.0f); // 5 minutes default
    
    // Get save info
    struct SaveInfo {
        std::string slotName;
        int day;
        int money;
        int reputation;
        std::string timestamp;
    };
    
    std::vector<SaveInfo> getAllSaves();
    void deleteSave(const std::string& slotName);
    
private:
    SaveManager();
    static SaveManager* s_instance;
    
    std::string getSaveFilePath(const std::string& slotName);
    void autoSaveCallback(float dt);
    
    bool _autoSaveEnabled;
    float _autoSaveInterval;
};

#endif // __SAVE_MANAGER_H__
