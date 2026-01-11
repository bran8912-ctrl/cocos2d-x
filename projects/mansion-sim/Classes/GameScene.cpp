#include "GameScene.h"
#include "SaveManager.h"
#include "SpriteFactory.h"
#include "AvatarGenerator.h"
#include "ProceduralArt.h"
#include "UIThemeManager.h"
#include "2d/CCParticleSystemQuad.h"
#include "2d/CCTransition.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    return GameScene::create();
}

bool GameScene::init()
{
    if (!Scene::init())
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // Background with gradient using theme manager
    auto bg = UIThemeManager::getInstance()->createThemedBackground("default");
    this->addChild(bg, -1);
    
    // Add decorative pattern
    auto pattern = ProceduralArt::getInstance()->generateDecorativePattern(
        visibleSize.width, visibleSize.height, "dots"
    );
    pattern->setOpacity(50);
    this->addChild(pattern, 0);
    
    // Add ambient particles for atmosphere
    auto particles = ParticleSystemQuad::create("Particles/Galaxy.plist");
    if (particles) {
        particles->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
        particles->setScale(0.5f);
        this->addChild(particles, 0);
    }
    
    // Setup UI
    setupUI();
    
    // Start game
    GameManager::getInstance()->startNewGame();
    updateUI();
    showEvent();
    
    return true;
}

void GameScene::setupUI()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // Top bar - Stats with enhanced visuals
    auto topBar = SpriteFactory::getInstance()->createPanel(Size(visibleSize.width, 100), "Status");
    topBar->setPosition(Vec2(origin.x, origin.y + visibleSize.height - 100));
    this->addChild(topBar, 1);
    
    // Money icon and label
    auto moneyIcon = ProceduralArt::getInstance()->generateIcon("money", 32);
    moneyIcon->setPosition(Vec2(origin.x + 80, origin.y + visibleSize.height - 50));
    this->addChild(moneyIcon, 2);
    
    moneyLabel = Label::createWithSystemFont("$100,000", "Arial", 24);
    moneyLabel->setPosition(Vec2(origin.x + 150, origin.y + visibleSize.height - 50));
    moneyLabel->setColor(Color3B(100, 255, 100));
    moneyLabel->enableShadow();
    this->addChild(moneyLabel, 2);
    
    // Reputation icon and label
    auto repIcon = ProceduralArt::getInstance()->generateIcon("star", 32);
    repIcon->setPosition(Vec2(origin.x + 320, origin.y + visibleSize.height - 50));
    this->addChild(repIcon, 2);
    
    reputationLabel = Label::createWithSystemFont("50", "Arial", 24);
    reputationLabel->setPosition(Vec2(origin.x + 380, origin.y + visibleSize.height - 50));
    reputationLabel->setColor(Color3B(255, 215, 0));
    reputationLabel->enableShadow();
    this->addChild(reputationLabel, 2);
    
    // Day label with building icon
    auto dayIcon = ProceduralArt::getInstance()->generateIcon("building", 32);
    dayIcon->setPosition(Vec2(origin.x + 560, origin.y + visibleSize.height - 50));
    this->addChild(dayIcon, 2);
    
    dayLabel = Label::createWithSystemFont("Day 1", "Arial", 24);
    dayLabel->setPosition(Vec2(origin.x + 640, origin.y + visibleSize.height - 50));
    dayLabel->setColor(Color3B::WHITE);
    dayLabel->enableShadow();
    this->addChild(dayLabel, 2);
    
    // Event Panel with enhanced visuals
    eventPanel = Layer::create();
    this->addChild(eventPanel, 3);
    
    auto eventBg = SpriteFactory::getInstance()->createPanel(Size(800, 450), "Event");
    eventBg->setPosition(Vec2(origin.x + visibleSize.width/2 - 400, origin.y + visibleSize.height/2 - 225));
    eventPanel->addChild(eventBg);
    
    eventTitle = Label::createWithSystemFont("Event Title", "Arial", 28);
    eventTitle->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2 + 170));
    eventTitle->setColor(Color3B(255, 200, 100));
    eventTitle->enableBold();
    eventTitle->enableShadow();
    eventPanel->addChild(eventTitle);
    
    eventDescription = Label::createWithSystemFont("Event description goes here...", "Arial", 20);
    eventDescription->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2 + 70));
    eventDescription->setDimensions(750, 0);
    eventDescription->setAlignment(TextHAlignment::CENTER);
    eventDescription->setColor(Color3B(220, 220, 255));
    eventPanel->addChild(eventDescription);
    
    // Choice buttons
    for (int i = 0; i < 3; i++) {
        auto button = ui::Button::create();
        button->setTitleText("Choice " + std::to_string(i + 1));
        button->setTitleFontSize(20);
        button->setTitleColor(Color3B::WHITE);
        button->setScale9Enabled(true);
        button->setCapInsets(Rect(10, 10, 10, 10));
        button->setContentSize(Size(350, 60));
        button->setColor(Color3B(70, 70, 100));
        button->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2 - 50 - i * 80));
        
        button->addClickEventListener([this, i](Ref* sender) {
            GameManager::getInstance()->handleDecision(i, [this](const AIEngine::DecisionResult& result) {
                showEventResult(result);
            });
            eventPanel->setVisible(false);
        });
        
        eventPanel->addChild(button);
        choiceButtons.push_back(button);
    }
    
    // Result Panel
    resultPanel = Layer::create();
    resultPanel->setVisible(false);
    this->addChild(resultPanel, 4);
    
    auto resultBg = LayerColor::create(Color4B(40, 60, 40, 230), 700, 300);
    resultBg->setPosition(Vec2(origin.x + visibleSize.width/2 - 350, origin.y + visibleSize.height/2 - 150));
    resultPanel->addChild(resultBg);
    
    resultText = Label::createWithSystemFont("Result...", "Arial", 22);
    resultText->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2 + 50));
    resultText->setDimensions(650, 0);
    resultText->setAlignment(TextHAlignment::CENTER);
    resultPanel->addChild(resultText);
    
    auto continueButton = ui::Button::create();
    continueButton->setTitleText("Continue");
    continueButton->setTitleFontSize(24);
    continueButton->setTitleColor(Color3B::WHITE);
    continueButton->setScale9Enabled(true);
    continueButton->setContentSize(Size(200, 60));
    continueButton->setColor(Color3B(50, 100, 50));
    continueButton->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2 - 80));
    
    continueButton->addClickEventListener([this](Ref* sender) {
        resultPanel->setVisible(false);
        nextDay();
    });
    
    resultPanel->addChild(continueButton);
    
    // Side Panel - Characters
    characterPanel = Layer::create();
    this->addChild(characterPanel, 2);
    
    auto sideBg = LayerColor::create(Color4B(35, 35, 55, 200), 300, visibleSize.height - 100);
    sideBg->setPosition(Vec2(origin.x + visibleSize.width - 300, origin.y + 10));
    characterPanel->addChild(sideBg);
    
    auto charTitle = Label::createWithSystemFont("👥 Staff & Guests", "Arial", 22);
    charTitle->setPosition(Vec2(origin.x + visibleSize.width - 150, origin.y + visibleSize.height - 120));
    charTitle->setColor(Color3B::CYAN);
    characterPanel->addChild(charTitle);
    
    // Scrollable character display area
    characterCardsLayer = Layer::create();
    characterPanel->addChild(characterCardsLayer);
    
    characterList = Label::createWithSystemFont("Loading...", "Arial", 16);
    characterList->setPosition(Vec2(origin.x + visibleSize.width - 150, origin.y + visibleSize.height - 250));
    characterList->setDimensions(280, 500);
    characterList->setAlignment(TextHAlignment::LEFT, TextVAlignment::TOP);
    characterPanel->addChild(characterList);
    
    // Save/Load Buttons
    auto saveButton = ui::Button::create();
    saveButton->setTitleText("Save");
    saveButton->setTitleFontSize(18);
    saveButton->setTitleColor(Color3B::WHITE);
    saveButton->setScale9Enabled(true);
    saveButton->setContentSize(Size(80, 40));
    saveButton->setColor(Color3B(50, 100, 50));
    saveButton->setPosition(Vec2(origin.x + visibleSize.width - 90, origin.y + 100));
    saveButton->addClickEventListener([this](Ref* sender) {
        SaveManager::getInstance()->saveGame("quicksave");
        // Show save confirmation with animation
        auto label = Label::createWithSystemFont("Game Saved!", "Arial", 24);
        label->setPosition(Director::getInstance()->getVisibleSize().width/2,
                          Director::getInstance()->getVisibleSize().height/2);
        label->setColor(Color3B::GREEN);
        this->addChild(label, 1000);
        label->runAction(Sequence::create(
            Spawn::create(
                FadeOut::create(2.0f),
                MoveBy::create(2.0f, Vec2(0, 50)),
                nullptr
            ),
            RemoveSelf::create(),
            nullptr
        ));
    });
    this->addChild(saveButton, 100);
    
    auto loadButton = ui::Button::create();
    loadButton->setTitleText("Load");
    loadButton->setTitleFontSize(18);
    loadButton->setTitleColor(Color3B::WHITE);
    loadButton->setScale9Enabled(true);
    loadButton->setContentSize(Size(80, 40));
    loadButton->setColor(Color3B(50, 50, 100));
    loadButton->setPosition(Vec2(origin.x + visibleSize.width - 200, origin.y + 100));
    loadButton->addClickEventListener([this](Ref* sender) {
        if (SaveManager::getInstance()->loadGame("quicksave")) {
            // Reload scene with transition
            auto scene = GameScene::createScene();
            auto transition = TransitionFade::create(0.5f, scene);
            Director::getInstance()->replaceScene(transition);
        } else {
            auto label = Label::createWithSystemFont("No Save Found!", "Arial", 24);
            label->setPosition(Director::getInstance()->getVisibleSize().width/2,
                              Director::getInstance()->getVisibleSize().height/2);
            label->setColor(Color3B::RED);
            this->addChild(label, 1000);
            label->runAction(Sequence::create(
                DelayTime::create(2.0f),
                FadeOut::create(1.0f),
                RemoveSelf::create(),
                nullptr
            ));
        }
    });
    this->addChild(loadButton, 100);
}

void GameScene::updateUI()
{
    auto gm = GameManager::getInstance();
    
    moneyLabel->setString("Money: $" + std::to_string(gm->getMoney()));
    reputationLabel->setString("Reputation: " + std::to_string(gm->getReputation()));
    dayLabel->setString("Day: " + std::to_string(gm->getDay()));
    
    // Update character list
    std::string charText = "STAFF:\n";
    for (const auto& staff : gm->getStaff()) {
        charText += staff.name + " (" + std::to_string(staff.satisfaction) + "%)\n";
    }
    charText += "\nGUESTS:\n";
    for (const auto& guest : gm->getGuests()) {
        charText += guest.name + " (" + std::to_string(guest.satisfaction) + "%)\n";
    }
    characterList->setString(charText);
}

void GameScene::showEvent()
{
    auto event = GameManager::getInstance()->getCurrentEvent();
    
    eventTitle->setString(event.title);
    eventDescription->setString(event.description);
    
    // Animate event panel appearance
    eventPanel->setScale(0.8f);
    eventPanel->setOpacity(0);
    eventPanel->runAction(Spawn::create(
        FadeIn::create(0.3f),
        ScaleTo::create(0.3f, 1.0f),
        nullptr
    ));
    
    for (size_t i = 0; i < choiceButtons.size() && i < event.choices.size(); i++) {
        choiceButtons[i]->setTitleText(event.choices[i]);
        choiceButtons[i]->setVisible(true);
    }
    
    // Hide unused buttons
    for (size_t i = event.choices.size(); i < choiceButtons.size(); i++) {
        choiceButtons[i]->setVisible(false);
    }
    
    eventPanel->setVisible(true);
}

void GameScene::showEventResult(const AIEngine::DecisionResult& result)
{
    // Add particle effect for good outcomes
    if (result.moneyChange > 0 || result.reputationChange > 10) {
        auto particles = ParticleSystemQuad::create("Particles/Fireworks.plist");
        if (particles) {
            auto visibleSize = Director::getInstance()->getVisibleSize();
            particles->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
            particles->setAutoRemoveOnFinish(true);
            this->addChild(particles, 200);
        }
    }
    
    std::string resultStr = result.outcome + "\n\n";
    
    if (result.moneyChange != 0) {
        resultStr += "Money: " + std::string(result.moneyChange > 0 ? "+" : "") + 
                     std::to_string(result.moneyChange) + "\n";
    }
    
    if (result.reputationChange != 0) {
        resultStr += "Reputation: " + std::string(result.reputationChange > 0 ? "+" : "") + 
                     std::to_string(result.reputationChange) + "\n";
    }
    
    resultText->setString(resultStr);
    resultPanel->setVisible(true);
    
    updateUI();
}

void GameScene::nextDay()
{
    auto gm = GameManager::getInstance();
    gm->nextDay();
    
    // Daily income from guests
    int dailyIncome = gm->getGuests().size() * 1000;
    gm->addMoney(dailyIncome);
    
    // Daily costs
    int dailyCosts = gm->getStaff().size() * 200;
    gm->addMoney(-dailyCosts);
    
    // Update characters
    gm->updateCharacters();
    
    // Random chance of new guest
    if (rand() % 100 < 50 + gm->getReputation() / 2) {
        gm->welcomeGuest();
    }
    
    // Generate new event
    gm->generateNewEvent();
    
    updateUI();
    showEvent();
}

void GameScene::createCharacterCards()
{
    characterCardsLayer->removeAllChildren();
    
    auto gm = GameManager::getInstance();
    int yPos = 150;
    
    // Display staff
    for (const auto& staff : gm->getStaff()) {
        auto card = CharacterSprite::createCharacterDisplay(
            staff.name, staff.role, staff.satisfaction
        );
        card->setPosition(Vec2(150, yPos));
        characterCardsLayer->addChild(card);
        yPos -= 100;
    }
    
    // Display guests
    for (const auto& guest : gm->getGuests()) {
        auto card = CharacterSprite::createCharacterDisplay(
            guest.name, guest.role, guest.satisfaction
        );
        card->setPosition(Vec2(150, yPos));
        characterCardsLayer->addChild(card);
        yPos -= 100;
    }
}

void GameScene::updateCharacterCards()
{
    createCharacterCards();
}