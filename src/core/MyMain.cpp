#include "MyMain.h"
#include "WorldManager.h"

int myMain() {
    int bpm = 1000;
    int delta = 100;
    auto window = std::make_unique<sf::RenderWindow>(sf::VideoMode({ 1000, 700 }), "JinProject");
    window->setFramerateLimit(60);

    auto worldManager = std::make_unique<WorldManager>(window.get());
    
    worldManager->createEntity(150, 250, 3, 20, 20, true);
    //worldManager->createEntity(50, 50, 3, 100, 100, false);

    sf::Clock deltaClock;
    while (window->isOpen()) {
        while (const auto event = window->pollEvent()) {

            if (event->is<sf::Event::Closed>()) {
                window->close();
                worldManager->destroy();
                return 0;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right)) {
                worldManager->moveRight();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left)) {
                worldManager->moveLeft();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up)) {
                worldManager->moveUp();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Down)) {
                worldManager->moveDown();
            }
        }

        worldManager->updateWorld();

        /*long c = std::clock();
        auto c_f = (float)c;
        if (c % bpm >= -delta && c % bpm <= delta) {
            shape.setFillColor(sf::Color::Green);
        }
        else {
            shape.setFillColor(sf::Color::Red);
        }*/

        window->clear();
        
        worldManager->renderEntities();

        window->display();
    }
    
    worldManager->destroy();

    return 0;
}