#include "WorldManager.h"
#include <iostream>

WorldManager::WorldManager() :
    worldDef{ b2DefaultWorldDef() },
    worldId{ b2CreateWorld(&worldDef) },
    levelManager{ std::make_unique<LevelManager>() },
    window{ std::make_unique<sf::RenderWindow>(sf::VideoMode({ 1000, 700 }), "JinProject") },
    camera { std::make_unique<sf::View>() }
{
    window->setFramerateLimit(60);
    camera->setCenter({0, 0});
    camera->setSize({ 1000, 700 });
    window->setView(*camera);
    b2World_SetGravity(worldId, b2Vec2{ 0, 0 });
}

void WorldManager::renderWorld() {
    levelManager->renderEntities(window.get());
}

void WorldManager::updateWorld() {
    levelManager->updateLevel(worldId);
    b2Vec2 playerPos = levelManager->getPlayerPosition();
    camera->setCenter({ playerPos.x, playerPos.y });
    window->setView(*camera);
}


void WorldManager::startGame() {
    levelManager->loadLevel(worldId, 0);

    while (window->isOpen()) {
        while (const auto event = window->pollEvent()) {

            if (event->is<sf::Event::Closed>()) {
                window->close();
                destroy();
                return;
            }
        }


        window->clear();
        updateWorld();
        renderWorld();
        window->display();
    }
    destroy();
}

void WorldManager::destroy() {
    b2DestroyWorld(worldId);
}