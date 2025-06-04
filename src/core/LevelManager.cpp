#include "LevelManager.h"
#include <iostream>
#include <fstream>

LevelManager::LevelManager():
    tempoTimePlayer{ std::clock() },
    levelGraph{ std::make_unique<Graph>() }
{
    tempoTimeEntities = tempoTimePlayer;

    int r = 25;
    beatIndicator.setRadius(r);
    beatIndicator.setPosition(sf::Vector2f(500 - r, 400 - r));

    if (!texture.loadFromFile("resources/sprites/fraise_animated.png")) {
        std::cerr << "Echec du chargement de la texture de test\n";
    }
}

void LevelManager::createPlayer(b2WorldId& worldId, float pos_x, float pos_y, bool showHitbox = false) {
    player = std::make_unique<Player>(worldId, pos_x, pos_y, &texture, showHitbox);
}

void LevelManager::createEnemy(b2WorldId& worldId, float pos_x, float pos_y, bool showHitbox = false) {
    enemies.push_back(std::make_unique<Enemy>(worldId, pos_x, pos_y, &texture, levelGraph.get(), showHitbox));
}

void LevelManager::createWall(b2WorldId& worldId, float pos_x, float pos_y, bool showHitbox) {
    auto wall = std::make_unique<Wall>(worldId, pos_x, pos_y, showHitbox);
    walls.push_back(std::move(wall));
}

void LevelManager::updateLevel(b2WorldId& worldId) {
    long clock = std::clock();

    updatePlayer(worldId);
    b2World_Step(worldId, timeStep, subStepCount);
    updateAll(clock);
    updateTempo(clock);
}

void LevelManager::updateAll(long clock) {
    for (const auto& enemy : enemies) {
        enemy->update(clock);
    }
    player->update(clock);
}



void LevelManager::updatePlayer(b2WorldId& worldId) {
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

void LevelManager::updateTempo(long clock) {

    if (clock >= (tempoTimePlayer - delta2) && clock <= (tempoTimePlayer + delta2)) {
        inTempo = true;
        beatIndicator.setFillColor(sf::Color::Green);
    }
    else if (clock >= (tempoTimePlayer - delta) && clock <= (tempoTimePlayer + delta)) {
        inTempo = true;
        beatIndicator.setFillColor(sf::Color::Red);
    }
    else {
        if (clock > (tempoTimePlayer - delta)) {
            tempoTimePlayer += tempoMS;
        }
        beatIndicator.setFillColor(sf::Color::Red);
        inTempo = false;
    }

    if (clock > tempoTimeEntities) {
        tempoTimeEntities += tempoMS;
        for (auto& enemy : enemies) {
            enemy->updateTempo(getPlayerPosition());
        }
    }
}

bool LevelManager::isInTempo() {
    return inTempo;
}

void LevelManager::renderEntities(sf::RenderWindow *window) {
    for (const auto& enemy : enemies) {
        enemy->renderEnemy(window);
    }
    player->renderEntity(window);
    for (const auto& wall : walls) {
        wall->renderWall(window);
    }
    window->draw(beatIndicator);
    window->draw(path_render);
}

void LevelManager::loadLevel(b2WorldId& worldId, int id) {
    std::string levelLine;
    std::ifstream levelFile("resources/Level0.txt");
    
    bool showHitboxes = true;
    int y = 0;
    int width = 0;

    while (std::getline(levelFile, levelLine)) {
        int x = 0;
        for (auto tile : levelLine) {
            switch (tile) {
            case '#':
                createWall(worldId, x* sizeMultiplier, y*sizeMultiplier, showHitboxes);
                goto noNode;
            case 'p':
                createPlayer(worldId, x* sizeMultiplier, y* sizeMultiplier, showHitboxes);
                break;
            case 'e':
                createEnemy(worldId, x* sizeMultiplier, y* sizeMultiplier, showHitboxes);
                break;
            case '-':
                break;
            default:
                break;
            }
            levelGraph->addNode(x, y);

            noNode:
            x++;
        }
        levelGraph->setWidth(x);
        y++;
    }
    levelGraph->setHeight(y);

    levelFile.close();

    levelGraph->initGraphTransitions();
    levelGraph->dumpGraph();
}

b2Vec2 LevelManager::getPlayerPosition() {
    return player->getPosition();
}