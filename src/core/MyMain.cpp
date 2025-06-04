#include "MyMain.h"
#include "WorldManager.h"

int myMain() {
    auto worldManager = std::make_unique<WorldManager>();
    
    worldManager->startGame();

    return 0;
}