#include "LevelManager.h"
#include <iostream>
#include <fstream>

LevelManager::LevelManager(WorldNotifier *wn):

    levelGraph{std::make_unique<Graph>()},
    world_notifier{wn},
    tempoTimePlayer{ std::clock() }
{
    tempoTimeEntities = tempoTimePlayer;

    int r = 25;
    beatIndicator.setRadius(r);
    beatIndicator.setPosition(sf::Vector2f(500 - r, 400 - r));

    if (!texture.loadFromFile("resources/sprites/fraise_animated.png")) {
        std::cerr << "Echec du chargement de la texture de test\n";
    }
}

void LevelManager::notifyDamage(int32_t hurtboxIndex, int damage) {
    if (hurtboxIndex == player->getShapeIndex()) {
        player->updateDamage(damage);
        return;
    }
    else if (enemies.contains(hurtboxIndex)) {
        enemies[hurtboxIndex]->updateDamage(damage);
        std::cout << "touched entity n" << hurtboxIndex << std::endl;
        return;
    }
    else {
        //terminate();
    }
}

void LevelManager::notifyDeath(int32_t hurtboxIndex) {
    if (hurtboxIndex == player->getShapeIndex()) {
        //Notify game finished
        return;
    }
    else if (enemies.contains(hurtboxIndex)) {
        enemies.erase(hurtboxIndex);
        return;
    }
    else { 
        //terminate(); 
    }
}

void LevelManager::createPlayer(b2WorldId& worldId, float pos_x, float pos_y, bool showHitbox = false) {
    player = std::make_unique<Player>(worldId, pos_x, pos_y, &texture, this, showHitbox);
}

void LevelManager::createEnemy(b2WorldId& worldId, float pos_x, float pos_y, bool showHitbox = false) {
    auto enemy = std::make_unique<Enemy>(worldId, pos_x, pos_y, &texture, levelGraph.get(), this, showHitbox);
    enemies[enemy->getShapeIndex()] = std::move(enemy);
}

void LevelManager::createWall(b2WorldId& worldId, float pos_x, float pos_y, bool showHitbox) {
    auto wall = std::make_unique<Wall>(worldId, pos_x, pos_y, showHitbox);
    walls.push_back(std::move(wall));
}

void LevelManager::createTransition(b2WorldId& worldId, float pos_x, float pos_y, direction dir) {
  auto transition = std::make_unique<LevelTransition>(worldId, pos_x, pos_y, dir);
  level_transitions.push_back(std::move(transition));
}

void LevelManager::updateLevel(b2WorldId& worldId) {
    long clock = std::clock();

    updatePlayer(worldId);
    b2World_Step(worldId, timeStep, subStepCount);
    updateAll(clock);
    updateTempo(clock);
}

void LevelManager::updateAll(long clock) {
    for (const auto& [index, enemy] : enemies) {
        enemy->update(clock);
    }
    for (const auto& transition : level_transitions) {
      if (transition->checkCollision()) {
        world_notifier->notifyTransition(transition->getDirection());
      }
    }
    player->update(clock);
}



void LevelManager::updatePlayer(b2WorldId& worldId) {
    
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
        for (auto& [index, enemy] : enemies) {
            enemy->updateTempo(getPlayerPosition());
        }
    }
}

bool LevelManager::isInTempo() {
    return inTempo;
}

void LevelManager::renderEntities(sf::RenderWindow *window) {
    for (const auto& [index,enemy] : enemies) {
        enemy->renderEnemy(window);
    }
    player->renderEntity(window);
    for (const auto& wall : walls) {
        wall->renderWall(window);
    }
    for (const auto& transition : level_transitions) {
      transition->draw(window);
    }
    window->draw(beatIndicator);
    window->draw(path_render);
}

void LevelManager::fileToMap(b2WorldId& worldId, const std::string& name) {
  std::string levelLine;
  std::ifstream levelFile(name);

  bool showHitboxes = true;
  int y = 0;
  int width = 0;

  while (std::getline(levelFile, levelLine)) {
    int x = 0;
    for (auto tile : levelLine) {
      switch (tile) {
        case '#':
          createWall(worldId, x * sizeMultiplier, y * sizeMultiplier, showHitboxes);
          goto noNode;
        case 'U':
          createTransition(worldId, (x + 1.f / 2.f) * sizeMultiplier,
                           (y - 1.f / 4.f) * sizeMultiplier, direction::UP);
          goto noNode;
        case 'D':
          createTransition(worldId, (x + 1.f / 2.f) * sizeMultiplier,
                           (y + 1.f / 4.f) * sizeMultiplier, direction::DOWN);
          goto noNode;
        case 'L':
          createTransition(worldId, (x-1.f/4.f) * sizeMultiplier, (y+1.f/2.f) * sizeMultiplier,  direction::LEFT);
          goto noNode;
        case 'R':
          createTransition(worldId, (x + 1.f / 4.f) * sizeMultiplier,
                           (y + 1.f / 2.f) * sizeMultiplier, direction::RIGHT);
          goto noNode;
        case 'p':
          createPlayer(worldId, x * sizeMultiplier, y * sizeMultiplier, showHitboxes);
          break;
        case 'e':
          createEnemy(worldId, x * sizeMultiplier, y * sizeMultiplier, showHitboxes);
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

void LevelManager::unloadLevel() {
  walls.clear();
  enemies.clear();
  level_transitions.clear();
  player = nullptr;
}

void LevelManager::loadLevel(b2WorldId& worldId, const std::string& name, direction dir) {
  unloadLevel();
  std::string filename = (std::string)"resources/rooms/" + name +".txt";
  fileToMap(worldId, filename);
  switch (dir) { 
    case direction::NONE:
      createPlayer(worldId,
                   (float)levelGraph->getWidth() / 2.f * sizeMultiplier,
                   (float)levelGraph->getHeight() / 2.f * sizeMultiplier, true);
      break;
    case direction::UP:
      createPlayer(worldId,
                   (float)levelGraph->getWidth() / 2.f * sizeMultiplier,
          (float)(levelGraph->getHeight() - 2) * (float)sizeMultiplier, true);
      break;
    case direction::DOWN:
      createPlayer(worldId,
                   (float)levelGraph->getWidth() / 2.f * sizeMultiplier,
                   2 * sizeMultiplier, true);
      break;
    case direction::LEFT:
      createPlayer(worldId,
                   (float)(levelGraph->getWidth() - 2) * (float)sizeMultiplier,
                   (float)levelGraph->getHeight() / 2.f * sizeMultiplier, true);
      break;
    case direction::RIGHT:
      createPlayer(worldId,
                   2 * sizeMultiplier,
                   (float)levelGraph->getHeight() / 2.f * sizeMultiplier, true);
      break;
  }
}

void LevelManager::loadFirstLevel(b2WorldId& worldId) {
  std::string levelLine;
  loadLevel(worldId, "TestStartRoom", direction::NONE);
}

b2Vec2 LevelManager::getPlayerPosition() {
    return player->getPosition();
}