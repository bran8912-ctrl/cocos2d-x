#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "GameManager.h"

class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    CREATE_FUNC(GameScene);
    
private:
    void setupUI();
    void updateUI();
    void showEvent();
    void showEventResult(const AIEngine::DecisionResult& result);
    void nextDay();
    
    // UI Elements
    cocos2d::Label* moneyLabel;
    cocos2d::Label* reputationLabel;
    cocos2d::Label* dayLabel;
    cocos2d::Label* statsLabel;
    
    cocos2d::Layer* eventPanel;
    cocos2d::Label* eventTitle;
    cocos2d::Label* eventDescription;
    std::vector<cocos2d::ui::Button*> choiceButtons;
    
    cocos2d::Layer* resultPanel;
    cocos2d::Label* resultText;
    
    cocos2d::Layer* characterPanel;
    cocos2d::Label* characterList;
};

#endif // __GAME_SCENE_H__
