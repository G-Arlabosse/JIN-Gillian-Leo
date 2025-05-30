#include "WorldManager.h"
#include <iostream>

WorldManager::WorldManager(sf::RenderWindow *window) :
    worldDef{ b2DefaultWorldDef() },
    worldId{ b2CreateWorld(&worldDef) },
    window { window }
{
    b2World_SetGravity(worldId, b2Vec2{ 0, 0 });
}

void WorldManager::moveRight() { entities[0]->move(100, 0); }
void WorldManager::moveLeft() { entities[0]->move(-100, 0); }
void WorldManager::moveUp() { entities[0]->move(0, -100); }
void WorldManager::moveDown() { entities[0]->move(0, 100); }

void WorldManager::createEntity(float pos_x, float pos_y, int hp, float hitboxSizeX, float hitboxSizeY, bool showHitbox=false) {
    auto hitboxSize = b2Vec2{ hitboxSizeX, hitboxSizeY };
    auto entity = std::make_unique<Entity>(worldId, pos_x, pos_y, hp, hitboxSize, showHitbox);
    entities.push_back(std::move(entity));
}

void WorldManager::renderEntities() {
    for (const auto& entity : entities) {
        entity->renderEntity(window);
    }
}

void WorldManager::updateWorld() {
    b2World_Step(worldId, 2*timeStep, subStepCount);
}

void WorldManager::destroy() {
    b2DestroyWorld(worldId);
}