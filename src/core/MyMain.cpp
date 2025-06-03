#include "MyMain.h"
#include "WorldManager.h"

int myMain() {
    auto window = std::make_unique<sf::RenderWindow>(sf::VideoMode({ 1000, 700 }), "JinProject");
    window->setFramerateLimit(60);

    auto worldManager = std::make_unique<WorldManager>(window.get());
    
    worldManager->createEntity(300, 300, 3, 20, 20, true);
    worldManager->createEntity(400, 200, 3, 20, 20, true);

    worldManager->createWall(200, 200, 20, 20, true);
    worldManager->createWall(240, 200, 20, 20, true);
    //worldManager->createEntity(50, 50, 3, 100, 100, false);

    sf::Clock deltaClock;
    while (window->isOpen()) {
        while (const auto event = window->pollEvent()) {

            if (event->is<sf::Event::Closed>()) {
                window->close();
                worldManager->destroy();
                return 0;
            }
        }

        worldManager->updateWorld();

        window->clear(sf::Color(128,128,128));
        worldManager->renderEntities();
        window->display();
    }
    
    worldManager->destroy();

    return 0;
}