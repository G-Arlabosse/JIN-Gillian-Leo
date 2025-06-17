#include "LevelManager.h"
#include <iostream>
#include <fstream>

LevelManager::LevelManager(WorldNotifier *wn, sf::RenderWindow *window, TextureManager* textureManager):

  levelGraph{std::make_unique<Graph>()},
  world_notifier{wn},
  tempoTimePlayer{ 0 },
  tempoTimeEntities{ 0 },
  window{ window },
  textureManager{ textureManager }
{
    //tempoTimeEntities = tempoTimePlayer;

    int r = 25;
    beatIndicator.setRadius(r);
    beatIndicator.setPosition(sf::Vector2f(500 - r, 400 - r));

    map.load("resources/tilemaps/TestStartRoom.tmx");
    layer = std::make_unique<MapLayer>(map, 0);

}

void LevelManager::notifyDamage(int32_t hurtboxIndex, int damage) {
    if (hurtboxIndex == player->getShapeIndex()) {
        player->updateDamage(damage);
        return;
    }
    else if (enemies.contains(hurtboxIndex)) {
        enemies[hurtboxIndex]->updateDamage(damage);
        return;
    }
    else {
        terminate();
    }
}

void LevelManager::notifyDeath(int32_t hurtboxIndex) {
    if (hurtboxIndex == player->getShapeIndex()) {
      callLoadLevel.isCalled = true;
      callLoadLevel.goBackToLobby = true;
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
    player = std::make_unique<Player>(worldId, pos_x + hitboxSize.x, pos_y + hitboxSize.y, textureManager, this, showHitbox);
}

void LevelManager::createEnemy(b2WorldId& worldId, float pos_x, float pos_y, bool showHitbox = false) {
  float x = pos_x + hitboxSize.x;
  float y = pos_y + hitboxSize.y;
  auto enemy = std::make_unique<Enemy>(worldId, x, y, textureName::STRAWBERRY, textureManager, levelGraph.get(), this, showHitbox);
  enemies[enemy->getShapeIndex()] = std::move(enemy);
}

void LevelManager::createWall(b2WorldId& worldId, float pos_x, float pos_y, bool showHitbox = false) {
    auto wall = std::make_unique<Wall>(worldId, pos_x + hitboxSize.x, pos_y + hitboxSize.y, hitboxSize, textureManager, showHitbox);
    walls.push_back(std::move(wall));
}

void LevelManager::createRoomTransition(b2WorldId& worldId, float pos_x, float pos_y, direction dir) {
  auto transition = std::make_unique<LevelTransition>(worldId, pos_x + hitboxSize.x, pos_y + hitboxSize.y, dir, textureManager);
  level_transitions.push_back(std::move(transition));
}

void LevelManager::createFloorTransition(b2WorldId& worldId, float pos_x, float pos_y, direction dir) {
  auto transition = std::make_unique<WorldTransition>(worldId, pos_x + hitboxSize.x, pos_y + hitboxSize.y, dir, textureManager);
  world_transitions.push_back(std::move(transition));
}

void LevelManager::updateLevel(b2WorldId& worldId) {
    long clock = std::clock();

    b2World_Step(worldId, timeStep, subStepCount);
    updateAll(clock);
    updateTempo(clock);

    if (callLoadLevel.isCalled) {
      if (callLoadLevel.goBackToLobby) {
        player = nullptr;
        world_notifier->loadLobby();
      }
      else {
        world_notifier->notifyTransition(callLoadLevel.dir);
      }
      callLoadLevel = {};
    }
}

void LevelManager::updateAll(long clock) {
  for (const auto& [index, enemy] : enemies) {
    enemy->update(clock);
  }
  for (const auto& transition : level_transitions) {
    if (enemies.size() == 0)
      transition->deactivate();
    if (transition->checkCollision()) {
      callLoadLevel.isCalled = true;
      callLoadLevel.dir = transition->getDirection();
      break;
    }
  }
  for (const auto& transition : world_transitions) {
    if (transition->checkCollision()) {
      callLoadLevel.isCalled = true;
      callLoadLevel.dir = transition->getDirection();
      break;
    }
  }
  player->update(clock, window, inTempo);
}

void LevelManager::initTempo() {
  tempoTimePlayer = std::clock();
  tempoTimeEntities = tempoTimePlayer;
}

//               #####-=============-#####
//-|-------------|----|------|------|----|-------------------------------------------------------
// 0            T-D  T-D'    T     T+D' T+D
void LevelManager::updateTempo(long clock) {
    if (clock > tempoTimePlayer + delta) {
        tempoTimePlayer += tempoMS;
        inTempo = false; 
        player->notifyEndTempo();
    }
    else if (clock > (tempoTimePlayer - delta2) && clock < (tempoTimePlayer + delta2)) {
        inTempo = true;
        beatIndicator.setFillColor(sf::Color::Green);
    }
    else if (clock > (tempoTimePlayer - delta) && clock > (tempoTimePlayer + delta2)) {
        inTempo = true;
        beatIndicator.setFillColor(sf::Color::Red);
    }
    else {
        inTempo = false;
    }


    if (clock > tempoTimeEntities) {
      tempoTimeEntities += tempoMS;
      for (const auto& [index, enemy] : enemies) {
        enemy->updateTempo(getPlayerPosition(), tempoMS);
      }
    }
}

bool LevelManager::isInTempo() {
    return inTempo;
}

void LevelManager::renderEntities(sf::RenderWindow *window) {
    window->draw(*layer);
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
    for (const auto& transition : world_transitions) {
      transition->draw(window);
    }
    window->draw(beatIndicator);
    window->draw(path_render);
}

void LevelManager::fileToMap(b2WorldId& worldId, const std::string& name,
                             bool cleared) {
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
          createRoomTransition(worldId, (x + 1.f / 2.f) * sizeMultiplier,
                           (y - 1.f / 4.f) * sizeMultiplier, direction::UP);
          goto noNode;
        case 'D':
          createRoomTransition(worldId, (x + 1.f / 2.f) * sizeMultiplier,
                           (y + 1.f / 4.f) * sizeMultiplier, direction::DOWN);
          goto noNode;
        case 'L':
          createRoomTransition(worldId, (x-1.f/4.f) * sizeMultiplier, (y+1.f/2.f) * sizeMultiplier,  direction::LEFT);
          goto noNode;
        case 'R':
          createRoomTransition(worldId, (x + 1.f / 4.f) * sizeMultiplier,
                           (y + 1.f / 2.f) * sizeMultiplier, direction::RIGHT);
          goto noNode;
        case 'p':
          createPlayer(worldId, x * sizeMultiplier, y * sizeMultiplier, showHitboxes);
          break;
        case 'e':
          if (!cleared)
            createEnemy(worldId, x * sizeMultiplier, y * sizeMultiplier, showHitboxes);
          break;
        case '+':
          createFloorTransition(worldId, x * sizeMultiplier, y * sizeMultiplier, direction::STAGE_DOWN);
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
  world_transitions.clear();
  levelGraph->clearGraph();
}

void LevelManager::loadLevel(b2WorldId& worldId, const std::string& name, direction dir, bool cleared) {
  unloadLevel();
  map.load((std::string) "resources/tilemaps/" + name + ".tmx");
  layer = std::make_unique<MapLayer>(map, 0);
  std::string filename = (std::string)"resources/rooms/" + name +".txt";
  fileToMap(worldId, filename, cleared);
  float pos_x, pos_y;
  switch (dir) {
    case direction::UP:
      pos_x = (float)levelGraph->getWidth() / 2.f * sizeMultiplier;
      pos_y = (float)(levelGraph->getHeight() - 2) * (float)sizeMultiplier;
      break;
    case direction::DOWN:
      pos_x = (float)levelGraph->getWidth() / 2.f * sizeMultiplier;
      pos_y = 2 * sizeMultiplier;
      break;
    case direction::LEFT:
      pos_x = (float)(levelGraph->getWidth() - 2) * (float)sizeMultiplier;
      pos_y = (float)levelGraph->getHeight() / 2.f * sizeMultiplier;
      break;
    case direction::RIGHT:
      pos_x = 2 * sizeMultiplier;
      pos_y = (float)levelGraph->getHeight() / 2.f * sizeMultiplier;
      break;
    default:
      pos_x = (float)levelGraph->getWidth() / 2.f * sizeMultiplier;
      pos_y = (float)levelGraph->getHeight() / 2.f * sizeMultiplier;
      break;
  }
  auto position = b2Vec2{ pos_x, pos_y };
  player->teleport(position);

}

void LevelManager::loadFirstLevel(b2WorldId& worldId) {
  std::string levelLine;
  loadLevel(worldId, "TestStartRoom", direction::NONE, true);
}

b2Vec2 LevelManager::getPlayerPosition() {
    return player->getPosition();
}

void LevelManager::setTempo(int bpm) {
  tempoMS = 1000.f * 60.f / bpm;
}