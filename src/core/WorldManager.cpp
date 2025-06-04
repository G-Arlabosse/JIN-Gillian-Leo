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

void WorldManager::createPlayer(float pos_x, float pos_y, bool showHitbox=false) {
    player = std::make_unique<Player>(worldId, pos_x, pos_y, &texture, showHitbox);
}

void WorldManager::createEnemy(float pos_x, float pos_y, bool showHitbox = false) {
    enemies.push_back(std::make_unique<Enemy>(worldId, pos_x, pos_y, &texture, showHitbox));
}

void WorldManager::createWall(float pos_x, float pos_y, float hitboxSizeX, float hitboxSizeY, bool showHitbox) {
    auto hitboxSize = b2Vec2{ hitboxSizeX, hitboxSizeY };
    auto wall = std::make_unique<Wall>(worldId, pos_x, pos_y, hitboxSize, showHitbox);
    walls.push_back(std::move(wall));
}

void WorldManager::renderEntities() {
    for (const auto& entity : enemies) {
        entity->renderEntity(window);
    }
    player->renderEntity(window);
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
    for (const auto& enemy : enemies) {
        enemy->update(clock);
    }
    player->update(clock);
}

void WorldManager::updatePlayer() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right)) {
        player->attack(worldId, b2Vec2(1, 0), 1);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left)) {
        player->attack(worldId, b2Vec2(-1, 0), 1);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up)) {
        player->attack(worldId, b2Vec2(0, -1), 1);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Down)) {
        player->attack(worldId, b2Vec2(0, 1), 1);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) {
        player->move(0, -200);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
        player->move(-200, 0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        player->move(0, 200);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        player->move(200, 0);
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
        for (auto& enemy : enemies) {
            enemy->updateTempo();
        }
    }
}

bool WorldManager::isInTempo() {
    return inTempo;
}

void WorldManager::destroy() {
    b2DestroyWorld(worldId);
}