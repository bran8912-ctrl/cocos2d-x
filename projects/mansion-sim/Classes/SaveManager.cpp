#include "SaveManager.h"
#include "base/CCUserDefault.h"
#include "platform/CCFileUtils.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include <ctime>

USING_NS_CC;

SaveManager* SaveManager::s_instance = nullptr;

SaveManager* SaveManager::getInstance() {
    if (!s_instance) {
        s_instance = new SaveManager();
    }
    return s_instance;
}

SaveManager::SaveManager() 
    : _autoSaveEnabled(false)
    , _autoSaveInterval(300.0f)
{
}

std::string SaveManager::getSaveFilePath(const std::string& slotName) {
    return FileUtils::getInstance()->getWritablePath() + "saves/" + slotName + ".json";
}

void SaveManager::saveGame(const std::string& slotName) {
    auto gm = GameManager::getInstance();
    
    // Create JSON document
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    
    // Save basic game state
    document.AddMember("version", 1, allocator);
    document.AddMember("day", gm->getDay(), allocator);
    document.AddMember("money", gm->getMoney(), allocator);
    document.AddMember("reputation", gm->getReputation(), allocator);
    
    // Save timestamp
    time_t now = time(0);
    char timeStr[64];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localtime(&now));
    rapidjson::Value timestamp;
    timestamp.SetString(timeStr, allocator);
    document.AddMember("timestamp", timestamp, allocator);
    
    // Save staff data
    rapidjson::Value staffArray(rapidjson::kArrayType);
    for (const auto& staff : gm->getStaff()) {
        rapidjson::Value staffObj(rapidjson::kObjectType);
        
        rapidjson::Value nameValue;
        nameValue.SetString(staff.name.c_str(), allocator);
        staffObj.AddMember("name", nameValue, allocator);
        
        rapidjson::Value roleValue;
        roleValue.SetString(staff.role.c_str(), allocator);
        staffObj.AddMember("role", roleValue, allocator);
        
        staffObj.AddMember("satisfaction", staff.satisfaction, allocator);
        staffObj.AddMember("loyalty", staff.loyalty, allocator);
        
        // Save traits
        rapidjson::Value traitsArray(rapidjson::kArrayType);
        for (const auto& trait : staff.traits) {
            rapidjson::Value traitValue;
            traitValue.SetString(trait.c_str(), allocator);
            traitsArray.PushBack(traitValue, allocator);
        }
        staffObj.AddMember("traits", traitsArray, allocator);
        
        staffArray.PushBack(staffObj, allocator);
    }
    document.AddMember("staff", staffArray, allocator);
    
    // Save guest data (similar pattern)
    rapidjson::Value guestArray(rapidjson::kArrayType);
    for (const auto& guest : gm->getGuests()) {
        rapidjson::Value guestObj(rapidjson::kObjectType);
        
        rapidjson::Value nameValue;
        nameValue.SetString(guest.name.c_str(), allocator);
        guestObj.AddMember("name", nameValue, allocator);
        
        guestObj.AddMember("satisfaction", guest.satisfaction, allocator);
        
        guestArray.PushBack(guestObj, allocator);
    }
    document.AddMember("guests", guestArray, allocator);
    
    // Convert to string
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);
    
    // Ensure directory exists
    std::string savePath = getSaveFilePath(slotName);
    std::string saveDir = FileUtils::getInstance()->getWritablePath() + "saves/";
    if (!FileUtils::getInstance()->isDirectoryExist(saveDir)) {
        FileUtils::getInstance()->createDirectory(saveDir);
    }
    
    // Write to file
    FileUtils::getInstance()->writeStringToFile(buffer.GetString(), savePath);
    
    log("Game saved to: %s", savePath.c_str());
}

bool SaveManager::loadGame(const std::string& slotName) {
    std::string savePath = getSaveFilePath(slotName);
    
    if (!FileUtils::getInstance()->isFileExist(savePath)) {
        log("Save file not found: %s", savePath.c_str());
        return false;
    }
    
    // Read file
    std::string jsonStr = FileUtils::getInstance()->getStringFromFile(savePath);
    
    // Parse JSON
    rapidjson::Document document;
    document.Parse(jsonStr.c_str());
    
    if (document.HasParseError()) {
        log("Error parsing save file");
        return false;
    }
    
    auto gm = GameManager::getInstance();
    gm->reset();
    
    // Load basic state
    if (document.HasMember("money")) {
        gm->addMoney(document["money"].GetInt() - gm->getMoney());
    }
    
    if (document.HasMember("reputation")) {
        gm->addReputation(document["reputation"].GetInt() - gm->getReputation());
    }
    
    if (document.HasMember("day")) {
        int targetDay = document["day"].GetInt();
        while (gm->getDay() < targetDay) {
            gm->nextDay();
        }
    }
    
    // Load staff
    if (document.HasMember("staff")) {
        gm->getStaff().clear();
        const rapidjson::Value& staffArray = document["staff"];
        for (rapidjson::SizeType i = 0; i < staffArray.Size(); i++) {
            const rapidjson::Value& staffObj = staffArray[i];
            
            Character staff;
            staff.name = staffObj["name"].GetString();
            staff.role = staffObj["role"].GetString();
            staff.satisfaction = staffObj["satisfaction"].GetInt();
            staff.loyalty = staffObj["loyalty"].GetInt();
            
            if (staffObj.HasMember("traits")) {
                const rapidjson::Value& traitsArray = staffObj["traits"];
                for (rapidjson::SizeType j = 0; j < traitsArray.Size(); j++) {
                    staff.traits.push_back(traitsArray[j].GetString());
                }
            }
            
            gm->getStaff().push_back(staff);
        }
    }
    
    // Load guests
    if (document.HasMember("guests")) {
        gm->getGuests().clear();
        const rapidjson::Value& guestArray = document["guests"];
        for (rapidjson::SizeType i = 0; i < guestArray.Size(); i++) {
            const rapidjson::Value& guestObj = guestArray[i];
            
            Character guest;
            guest.name = guestObj["name"].GetString();
            guest.role = "guest";
            guest.satisfaction = guestObj["satisfaction"].GetInt();
            
            gm->getGuests().push_back(guest);
        }
    }
    
    log("Game loaded from: %s", savePath.c_str());
    return true;
}

bool SaveManager::hasSaveData(const std::string& slotName) {
    std::string savePath = getSaveFilePath(slotName);
    return FileUtils::getInstance()->isFileExist(savePath);
}

void SaveManager::enableAutoSave(bool enable, float interval) {
    _autoSaveEnabled = enable;
    _autoSaveInterval = interval;
    
    if (enable) {
        Director::getInstance()->getScheduler()->schedule(
            CC_SCHEDULE_SELECTOR(SaveManager::autoSaveCallback),
            this,
            interval,
            false
        );
    } else {
        Director::getInstance()->getScheduler()->unschedule(
            CC_SCHEDULE_SELECTOR(SaveManager::autoSaveCallback),
            this
        );
    }
}

void SaveManager::autoSaveCallback(float dt) {
    if (_autoSaveEnabled) {
        saveGame("autosave");
        log("Auto-save completed");
    }
}

std::vector<SaveManager::SaveInfo> SaveManager::getAllSaves() {
    std::vector<SaveInfo> saves;
    
    std::string saveDir = FileUtils::getInstance()->getWritablePath() + "saves/";
    
    // This is simplified - in production you'd scan the directory
    // For now, check common save slots
    std::vector<std::string> slots = {"quicksave", "autosave", "slot1", "slot2", "slot3"};
    
    for (const auto& slot : slots) {
        std::string savePath = getSaveFilePath(slot);
        if (FileUtils::getInstance()->isFileExist(savePath)) {
            std::string jsonStr = FileUtils::getInstance()->getStringFromFile(savePath);
            rapidjson::Document document;
            document.Parse(jsonStr.c_str());
            
            if (!document.HasParseError()) {
                SaveInfo info;
                info.slotName = slot;
                info.day = document.HasMember("day") ? document["day"].GetInt() : 0;
                info.money = document.HasMember("money") ? document["money"].GetInt() : 0;
                info.reputation = document.HasMember("reputation") ? document["reputation"].GetInt() : 0;
                info.timestamp = document.HasMember("timestamp") ? document["timestamp"].GetString() : "Unknown";
                
                saves.push_back(info);
            }
        }
    }
    
    return saves;
}

void SaveManager::deleteSave(const std::string& slotName) {
    std::string savePath = getSaveFilePath(slotName);
    if (FileUtils::getInstance()->isFileExist(savePath)) {
        FileUtils::getInstance()->removeFile(savePath);
        log("Deleted save: %s", slotName.c_str());
    }
}
