#include "AppDelegate.h"
#include "GameScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate() {}

void AppDelegate::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0};
    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching()
{
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    
    if(!glview) {
        glview = GLViewImpl::createWithRect("Mansion Tycoon", Rect(0, 0, 1280, 720));
        director->setOpenGLView(glview);
    }

    director->setDisplayStats(true);
    director->setAnimationInterval(1.0f / 60);

    auto scene = GameScene::createScene();
    director->runWithScene(scene);

    return true;
}

void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();
}

void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();
}
