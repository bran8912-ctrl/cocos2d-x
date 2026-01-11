#include "AppDelegate.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_EMSCRIPTEN)
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

USING_NS_CC;

int main(int argc, char *argv[])
{
    AppDelegate app;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_EMSCRIPTEN)
    // For Emscripten builds
    return Application::getInstance()->run();
#else
    return app.run();
#endif
}
