#include "WorldManager.h"
#include <iostream>

WorldManager::WorldManager(sf::RenderWindow *window) :
    worldDef{ b2DefaultWorldDef() },
    worldId{ b2CreateWorld(&worldDef) },
    window { window }
{
    b2World_SetGravity(worldId, b2Vec2{ 0, 0 });

    if (!texture.loadFromFile("resources/testSprite.png")) {
        std::cerr << "Échec du chargement de la texture de test\n";
    }

    int r = 20;
    beatIndicator.setRadius(r);
    beatIndicator.setPosition(sf::Vector2f(window->getSize().x/2 - r, window->getSize().y/2- r));
}

void WorldManager::moveRight() { entities[0]->move(100, 0); }
void WorldManager::moveLeft() { entities[0]->move(-100, 0); }
void WorldManager::moveUp() { entities[0]->move(0, -100); }
void WorldManager::moveDown() { entities[0]->move(0, 100); }

void WorldManager::createEntity(float pos_x, float pos_y, int hp, float hitboxSizeX, float hitboxSizeY, bool showHitbox=false) {
    auto hitboxSize = b2Vec2{ hitboxSizeX, hitboxSizeY };
    Entity entity0(worldId, pos_x, pos_y, hp, hitboxSize, &texture, showHitbox);
    auto entity = std::make_unique<Entity>(worldId, pos_x, pos_y, hp, hitboxSize, &texture, showHitbox);
    entities.push_back(std::move(entity));
}

void WorldManager::createWall(float pos_x, float pos_y, float hitboxSizeX, float hitboxSizeY, bool showHitbox) {
    auto hitboxSize = b2Vec2{ hitboxSizeX, hitboxSizeY };
    auto wall = std::make_unique<Wall>(worldId, pos_x, pos_y, hitboxSize, showHitbox);
    walls.push_back(std::move(wall));
}

void WorldManager::renderEntities() {
    for (const auto& entity : entities) {
        entity->renderEntity(window);
    }
    for (const auto& wall : walls) {
        wall->renderWall(window);
    }
    window->draw(beatIndicator);
}

void WorldManager::updateWorld() {
    b2World_Step(worldId, 2*timeStep, subStepCount);

    long c = std::clock();
    long modC = c % bpm;
    if (modC >= (bpm - delta2) || modC <= delta2) {
        inTempo = true;
        beatIndicator.setFillColor(sf::Color::Green);
    }
    else if (modC >= (bpm - delta) || modC <= delta) {
        inTempo = true;
        beatIndicator.setFillColor(sf::Color::Red);
    }
    else {
        beatIndicator.setFillColor(sf::Color::Red);
        inTempo = false;
    }
}

bool WorldManager::isInTempo() {
    return inTempo;
}

void WorldManager::destroy() {
    b2DestroyWorld(worldId);
}