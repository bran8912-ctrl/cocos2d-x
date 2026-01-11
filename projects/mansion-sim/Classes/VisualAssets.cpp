#include "VisualAssets.h"
#include <sstream>
#include <cmath>
#include <algorithm>

USING_NS_CC;

// ==================== AvatarGenerator ====================

AvatarGenerator* AvatarGenerator::s_instance = nullptr;

AvatarGenerator* AvatarGenerator::getInstance() {
    if (!s_instance) {
        s_instance = new AvatarGenerator();
    }
    return s_instance;
}

AvatarGenerator::AvatarGenerator() {}

std::string AvatarGenerator::nameToSeed(const std::string& name) {
    unsigned int hash = 0;
    for (char c : name) {
        hash = ((hash << 5) - hash) + c;
    }
    return std::to_string(hash);
}

Color3B AvatarGenerator::hashToColor(const std::string& seed) {
    unsigned int hash = std::stoul(seed);
    unsigned char r = (hash >> 16) & 0xFF;
    unsigned char g = (hash >> 8) & 0xFF;
    unsigned char b = hash & 0xFF;
    
    // Ensure vibrant colors
    int lum = (r * 299 + g * 587 + b * 114) / 1000;
    if (lum < 100) {
        r = std::min(255, (int)r + 100);
        g = std::min(255, (int)g + 100);
        b = std::min(255, (int)b + 100);
    } else if (lum > 200) {
        r = std::max(0, (int)r - 50);
        g = std::max(0, (int)g - 50);
        b = std::max(0, (int)b - 50);
    }
    
    return Color3B(r, g, b);
}

std::string AvatarGenerator::generateAvatarSVG(const std::string& name, const std::string& role) {
    std::string seed = nameToSeed(name);
    Color3B color = hashToColor(seed);
    
    // Generate simple SVG avatar with initials
    std::string initials;
    for (char c : name) {
        if (initials.length() < 2 && c != ' ') {
            initials += c;
        }
    }
    
    std::ostringstream svg;
    svg << "<svg width='100' height='100' xmlns='http://www.w3.org/2000/svg'>"
        << "<circle cx='50' cy='50' r='50' fill='rgb(" << (int)color.r << "," << (int)color.g << "," << (int)color.b << ")'/>"
        << "<circle cx='50' cy='50' r='48' fill='rgba(255,255,255,0.1)'/>"
        << "<text x='50' y='65' font-size='40' font-weight='bold' text-anchor='middle' fill='white'>" 
        << initials << "</text>"
        << "</svg>";
    
    return svg.str();
}

Color3B AvatarGenerator::getRoleColor(const std::string& role) {
    if (role == "staff") {
        return Color3B(100, 150, 255);  // Blue
    } else if (role == "guest") {
        return Color3B(255, 200, 100);  // Orange
    } else if (role == "vip") {
        return Color3B(255, 215, 0);    // Gold
    }
    return Color3B(150, 150, 150);      // Gray
}

std::string AvatarGenerator::getRoleIcon(const std::string& role) {
    if (role == "staff") {
        return "👔";
    } else if (role == "guest") {
        return "👤";
    } else if (role == "vip") {
        return "👑";
    }
    return "🙂";
}

// ==================== IconManager ====================

IconManager* IconManager::s_instance = nullptr;

IconManager* IconManager::getInstance() {
    if (!s_instance) {
        s_instance = new IconManager();
    }
    return s_instance;
}

IconManager::IconManager() {}

std::string IconManager::getIcon(const std::string& iconName) {
    // Font Awesome emoji equivalents
    if (iconName == "money") return "💰";
    if (iconName == "reputation") return "⭐";
    if (iconName == "staff") return "👥";
    if (iconName == "guest") return "🎭";
    if (iconName == "heart") return "❤️";
    if (iconName == "star") return "⭐";
    if (iconName == "trophy") return "🏆";
    if (iconName == "clock") return "🕐";
    if (iconName == "building") return "🏢";
    if (iconName == "handshake") return "🤝";
    if (iconName == "chart") return "📊";
    if (iconName == "settings") return "⚙️";
    
    return "•";
}

std::string IconManager::getColoredIcon(const std::string& iconName, Color3B color) {
    // In a real implementation, this would apply color styling
    // For now, just return the icon
    return getIcon(iconName);
}

std::string IconManager::getIconByType(IconType type) {
    switch (type) {
        case IconType::MONEY: return "💰";
        case IconType::REPUTATION: return "⭐";
        case IconType::STAFF: return "👥";
        case IconType::GUEST: return "🎭";
        case IconType::HEART: return "❤️";
        case IconType::STAR: return "✨";
        case IconType::TROPHY: return "🏆";
        case IconType::CLOCK: return "🕐";
        case IconType::BUILDING: return "🏢";
        case IconType::HANDSHAKE: return "🤝";
        case IconType::CHART: return "📊";
        case IconType::SETTINGS: return "⚙️";
        default: return "•";
    }
}

std::string IconManager::iconToUnicode(IconType type) {
    return getIconByType(type);
}

// ==================== VisualEffectGenerator ====================

VisualEffectGenerator* VisualEffectGenerator::s_instance = nullptr;

VisualEffectGenerator* VisualEffectGenerator::getInstance() {
    if (!s_instance) {
        s_instance = new VisualEffectGenerator();
    }
    return s_instance;
}

VisualEffectGenerator::VisualEffectGenerator() {}

Node* VisualEffectGenerator::createDecisionEffects(Vec2 position, int moneyChange, int reputationChange) {
    auto container = Node::create();
    
    // Money effect
    if (moneyChange != 0) {
        std::string moneyText = (moneyChange > 0 ? "+" : "") + std::to_string(moneyChange);
        Color3B color = moneyChange > 0 ? Color3B::GREEN : Color3B::RED;
        
        auto moneyLabel = Label::createWithSystemFont(moneyText, "Arial", 24);
        moneyLabel->setColor(color);
        moneyLabel->setPosition(position);
        container->addChild(moneyLabel);
        
        // Float up animation
        auto moveUp = MoveBy::create(1.0f, Vec2(0, 50));
        auto fadeOut = FadeOut::create(1.0f);
        auto spawn = Spawn::create(moveUp, fadeOut, nullptr);
        moneyLabel->runAction(spawn);
    }
    
    // Reputation effect
    if (reputationChange != 0) {
        std::string repText = (reputationChange > 0 ? "+" : "") + std::to_string(reputationChange) + " Rep";
        Color3B color = reputationChange > 0 ? Color3B::YELLOW : Color3B::MAGENTA;
        
        auto repLabel = Label::createWithSystemFont(repText, "Arial", 20);
        repLabel->setColor(color);
        repLabel->setPosition(position + Vec2(0, -30));
        container->addChild(repLabel);
        
        auto moveUp = MoveBy::create(1.0f, Vec2(0, 40));
        auto fadeOut = FadeOut::create(1.0f);
        auto spawn = Spawn::create(moveUp, fadeOut, nullptr);
        repLabel->runAction(spawn);
    }
    
    return container;
}

Sprite* VisualEffectGenerator::createSatisfactionBar(int satisfaction, Size size) {
    auto barBg = DrawNode::create();
    barBg->drawSolidRect(Vec2(0, 0), Vec2(size.width, size.height), Color4F(0.3f, 0.3f, 0.3f, 1.0f));
    
    // Clamp satisfaction between 0-100
    satisfaction = std::max(0, std::min(100, satisfaction));
    float fillWidth = (satisfaction / 100.0f) * size.width;
    
    // Color based on satisfaction
    Color4F fillColor;
    if (satisfaction < 30) {
        fillColor = Color4F(1.0f, 0.2f, 0.2f, 1.0f);  // Red
    } else if (satisfaction < 60) {
        fillColor = Color4F(1.0f, 0.8f, 0.2f, 1.0f);  // Yellow
    } else {
        fillColor = Color4F(0.2f, 1.0f, 0.2f, 1.0f);  // Green
    }
    
    barBg->drawSolidRect(Vec2(0, 0), Vec2(fillWidth, size.height), fillColor);
    
    return barBg;
}

Sprite* VisualEffectGenerator::createReputationBadge(int reputation) {
    // Create a visual badge based on reputation level
    std::string badgeText;
    Color3B color;
    
    if (reputation >= 90) {
        badgeText = "★★★★★";
        color = Color3B(255, 215, 0);      // Gold
    } else if (reputation >= 70) {
        badgeText = "★★★★";
        color = Color3B(192, 192, 192);    // Silver
    } else if (reputation >= 50) {
        badgeText = "★★★";
        color = Color3B(205, 127, 50);     // Bronze
    } else if (reputation >= 30) {
        badgeText = "★★";
        color = Color3B(100, 100, 100);
    } else {
        badgeText = "★";
        color = Color3B(100, 100, 100);
    }
    
    auto label = Label::createWithSystemFont(badgeText, "Arial", 20);
    label->setColor(color);
    
    return label;
}

Label* VisualEffectGenerator::createFloatingText(const std::string& text, Color3B color) {
    auto label = Label::createWithSystemFont(text, "Arial", 18);
    label->setColor(color);
    
    return label;
}

// ==================== CharacterSprite ====================

Node* CharacterSprite::createCharacterDisplay(const std::string& name, 
                                               const std::string& role,
                                               int satisfaction) {
    auto container = Node::create();
    container->setContentSize(Size(150, 120));
    
    // Avatar background
    auto bg = LayerColor::create(Color4B(50, 50, 70, 200), 150, 120);
    container->addChild(bg, 1);
    
    // Role icon
    std::string icon = IconManager::getInstance()->getIconByType(
        role == "staff" ? IconManager::IconType::STAFF : IconManager::IconType::GUEST
    );
    
    auto iconLabel = Label::createWithSystemFont(icon, "Arial", 40);
    iconLabel->setPosition(Vec2(75, 80));
    container->addChild(iconLabel, 2);
    
    // Name
    auto nameLabel = Label::createWithSystemFont(name, "Arial", 14);
    nameLabel->setPosition(Vec2(75, 40));
    nameLabel->setDimensions(140, 0);
    nameLabel->setAlignment(TextHAlignment::CENTER);
    nameLabel->setColor(Color3B::WHITE);
    container->addChild(nameLabel, 2);
    
    // Satisfaction bar
    int barHeight = 8;
    auto satBar = VisualEffectGenerator::getInstance()->createSatisfactionBar(satisfaction, Size(140, barHeight));
    satBar->setPosition(Vec2(5, 10));
    container->addChild(satBar, 2);
    
    return container;
}

Node* CharacterSprite::createCharacterCard(const std::string& name,
                                            const std::string& role,
                                            const std::vector<std::string>& traits,
                                            int satisfaction,
                                            int loyalty) {
    auto container = Layer::create();
    container->setContentSize(Size(300, 250));
    
    // Card background
    auto cardBg = LayerColor::create(Color4B(40, 40, 60, 230), 300, 250);
    container->addChild(cardBg, 1);
    
    // Header with role icon and name
    Color3B roleColor = CharacterSprite::getRoleColor(role);
    std::string roleIcon = CharacterSprite::getRoleIcon(role);
    
    auto headerBg = LayerColor::create(Color4B(roleColor.r / 2, roleColor.g / 2, roleColor.b / 2, 200), 300, 60);
    headerBg->setPosition(Vec2(0, 190));
    container->addChild(headerBg, 2);
    
    auto iconLabel = Label::createWithSystemFont(roleIcon, "Arial", 30);
    iconLabel->setPosition(Vec2(30, 220));
    container->addChild(iconLabel, 3);
    
    auto nameLabel = Label::createWithSystemFont(name, "Arial", 20);
    nameLabel->setColor(Color3B::WHITE);
    nameLabel->setPosition(Vec2(80, 220));
    container->addChild(nameLabel, 3);
    
    auto roleLabel = Label::createWithSystemFont(role, "Arial", 12);
    roleLabel->setColor(Color3B(200, 200, 200));
    roleLabel->setPosition(Vec2(80, 200));
    container->addChild(roleLabel, 3);
    
    // Stats section
    int yPos = 170;
    
    // Satisfaction
    auto satLabel = Label::createWithSystemFont("Satisfaction: " + std::to_string(satisfaction) + "%", "Arial", 14);
    satLabel->setColor(Color3B::YELLOW);
    satLabel->setPosition(Vec2(20, yPos));
    satLabel->setAlignment(TextHAlignment::LEFT);
    container->addChild(satLabel, 3);
    yPos -= 25;
    
    // Loyalty
    auto loyalLabel = Label::createWithSystemFont("Loyalty: " + std::to_string(loyalty) + "%", "Arial", 14);
    loyalLabel->setColor(Color3B::CYAN);
    loyalLabel->setPosition(Vec2(20, yPos));
    loyalLabel->setAlignment(TextHAlignment::LEFT);
    container->addChild(loyalLabel, 3);
    yPos -= 30;
    
    // Traits
    auto traitsLabel = Label::createWithSystemFont("Traits:", "Arial", 12);
    traitsLabel->setColor(Color3B(150, 150, 150));
    traitsLabel->setPosition(Vec2(20, yPos));
    traitsLabel->setAlignment(TextHAlignment::LEFT);
    container->addChild(traitsLabel, 3);
    yPos -= 20;
    
    for (const auto& trait : traits) {
        auto traitLabel = Label::createWithSystemFont("• " + trait, "Arial", 11);
        traitLabel->setColor(Color3B(100, 200, 100));
        traitLabel->setPosition(Vec2(25, yPos));
        traitLabel->setAlignment(TextHAlignment::LEFT);
        container->addChild(traitLabel, 3);
        yPos -= 18;
    }
    
    return container;
}

Color3B CharacterSprite::getRoleColor(const std::string& role) {
    if (role == "staff") {
        return Color3B(100, 150, 255);
    } else if (role == "guest") {
        return Color3B(255, 200, 100);
    } else if (role == "vip") {
        return Color3B(255, 215, 0);
    }
    return Color3B(150, 150, 150);
}

std::string CharacterSprite::getRoleIcon(const std::string& role) {
    if (role == "staff") {
        return "👔";
    } else if (role == "guest") {
        return "👤";
    } else if (role == "vip") {
        return "👑";
    }
    return "🙂";
}
