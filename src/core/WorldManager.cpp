#include "WorldManager.h"
#include <iostream>

WorldManager::WorldManager(sf::RenderWindow *window) :
    worldDef{ b2DefaultWorldDef() },
    worldId{ b2CreateWorld(&worldDef) },
    window { window },
    tempoTimePlayer { std::clock() }
{
    tempoTimeEntities = tempoTimePlayer;

    b2World_SetGravity(worldId, b2Vec2{ 0, 0 });

    if (!texture.loadFromFile("resources/sprites/fraise_animated.png")) {
        std::cerr << "Échec du chargement de la texture de test\n";
    }

    int r = 25;
    beatIndicator.setRadius(r);
    beatIndicator.setPosition(sf::Vector2f(window->getSize().x/2 - r, window->getSize().y*5/6- r));
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
    for (const auto& hitbox : hitboxes) {
        hitbox->draw(window, sf::Color::Yellow);
    }
    window->draw(beatIndicator);
}

void WorldManager::updateWorld() {
    long clock = std::clock();

    updatePlayer();
    b2World_Step(worldId, timeStep, subStepCount);
    updateAll(clock);
    updateTempo(clock);
}

void WorldManager::updateAll(long clock) {
    for (auto& entity : entities) {
        entity->update(clock);
    }
}

void WorldManager::updatePlayer() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right)) {
        entities[0]->attack(worldId, b2Vec2(1, 0), 1);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left)) {
        entities[0]->attack(worldId, b2Vec2(-1, 0), 1);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up)) {
        entities[0]->attack(worldId, b2Vec2(0, -1), 1);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Down)) {
        entities[0]->attack(worldId, b2Vec2(0, 1), 1);
    }
}

void WorldManager::updateTempo(long clock) {

    if (clock >= (tempoTimePlayer - delta2) && clock <= (tempoTimePlayer + delta2)) {
        inTempo = true;
        beatIndicator.setFillColor(sf::Color::Green);
    }
    else if (clock >= (tempoTimePlayer - delta) && clock <= (tempoTimePlayer + delta)) {
        inTempo = true;
        beatIndicator.setFillColor(sf::Color::Red);
    }
    else  {
        if (clock > (tempoTimePlayer - delta)) {
            tempoTimePlayer += bpm;
        }
        beatIndicator.setFillColor(sf::Color::Red);
        inTempo = false;
    }

    if (clock > tempoTimeEntities) {
        tempoTimeEntities += bpm;
        for (auto& entity : entities) {
            entity->updateTempo();
        }
    }
}

bool WorldManager::isInTempo() {
    return inTempo;
}

void WorldManager::destroy() {
    b2DestroyWorld(worldId);
}