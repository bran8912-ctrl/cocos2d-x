cmake_minimum_required(VERSION 3.10)

project(MansionSim)

set(CMAKE_CXX_STANDARD 17)

# Cocos2d-x
set(COCOS2DX_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../..)
set(CMAKE_MODULE_PATH ${COCOS2DX_ROOT_PATH}/cmake/Modules/)

include(CocosBuildSet)
add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos ${CMAKE_CURRENT_BINARY_DIR}/cocos)

# Game sources
file(GLOB_RECURSE GAME_SOURCES 
    "Classes/*.cpp"
)

file(GLOB_RECURSE GAME_HEADERS 
    "Classes/*.h"
)

# List key files for clarity
set(KEY_SOURCES
    Classes/AppDelegate.cpp
    Classes/GameScene.cpp
    Classes/GameManager.cpp
    Classes/AIEngine.cpp
    Classes/SaveManager.cpp
    Classes/VisualAssets.cpp
    Classes/main.cpp
)

add_executable(${PROJECT_NAME} 
    ${GAME_SOURCES}
    ${GAME_HEADERS}
)

target_link_libraries(${PROJECT_NAME} cocos2d)

target_include_directories(${PROJECT_NAME} PRIVATE 
    Classes
    ${COCOS2DX_ROOT_PATH}/cocos
)
