#ifndef __VISUAL_ASSETS_H__
#define __VISUAL_ASSETS_H__

#include "cocos2d.h"
#include <string>
#include <vector>

// Procedural avatar generation for characters
class AvatarGenerator {
public:
    static AvatarGenerator* getInstance();
    
    // Generate SVG avatar based on character traits
    std::string generateAvatarSVG(const std::string& name, const std::string& role);
    
    // Get avatar color based on role
    cocos2d::Color3B getRoleColor(const std::string& role);
    
    // Get icon emoji for role
    std::string getRoleIcon(const std::string& role);
    
private:
    AvatarGenerator();
    static AvatarGenerator* s_instance;
    
    std::string nameToSeed(const std::string& name);
    cocos2d::Color3B hashToColor(const std::string& seed);
};

// UI Icon system
class IconManager {
public:
    static IconManager* getInstance();
    
    // Get icon character from Font Awesome equivalent
    std::string getIcon(const std::string& iconName);
    
    // Get icon with color
    std::string getColoredIcon(const std::string& iconName, cocos2d::Color3B color);
    
    enum class IconType {
        MONEY,
        REPUTATION,
        STAFF,
        GUEST,
        HEART,
        STAR,
        TROPHY,
        CLOCK,
        BUILDING,
        HANDSHAKE,
        CHART,
        SETTINGS
    };
    
    std::string getIconByType(IconType type);
    
private:
    IconManager();
    static IconManager* s_instance;
    
    std::string iconToUnicode(IconType type);
};

// Procedural visual effects
class VisualEffectGenerator {
public:
    static VisualEffectGenerator* getInstance();
    
    // Create particle system for decision outcomes
    cocos2d::Node* createDecisionEffects(cocos2d::Vec2 position, int moneyChange, int reputationChange);
    
    // Create character satisfaction indicator
    cocos2d::Sprite* createSatisfactionBar(int satisfaction, cocos2d::Size size);
    
    // Create reputation badge
    cocos2d::Sprite* createReputationBadge(int reputation);
    
    // Floating text effect for money/reputation changes
    cocos2d::Label* createFloatingText(const std::string& text, cocos2d::Color3B color);
    
private:
    VisualEffectGenerator();
    static VisualEffectGenerator* s_instance;
    
    cocos2d::DrawNode* createGradientBar(float width, float height, 
                                         cocos2d::Color3B color1, cocos2d::Color3B color2);
};

// Character visual representation
class CharacterSprite {
public:
    static cocos2d::Node* createCharacterDisplay(const std::string& name, 
                                                  const std::string& role,
                                                  int satisfaction);
    
    static cocos2d::Node* createCharacterCard(const std::string& name,
                                               const std::string& role,
                                               const std::vector<std::string>& traits,
                                               int satisfaction,
                                               int loyalty);
    
private:
    static cocos2d::Color3B getRoleColor(const std::string& role);
    static std::string getRoleIcon(const std::string& role);
};

#endif // __VISUAL_ASSETS_H__
