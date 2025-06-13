#include "WorldManager.h"
#include <iostream>

using namespace std;
using namespace std::literals;

WorldManager::WorldManager() :
  worldDef{ b2DefaultWorldDef() },
  worldId{ b2CreateWorld(&worldDef) },
  window{ std::make_unique<sf::RenderWindow>(sf::VideoMode({ 1000, 700 }), "JinProject") },
  camera { std::make_unique<sf::View>() },
  textureManager{ std::make_unique<TextureManager>() }
{
    window->setFramerateLimit(60);
    camera->setCenter({0, 0});
    camera->setSize({ 1000, 700 });
    window->setView(*camera);
    b2World_SetGravity(worldId, b2Vec2{ 0, 0 });
    map = makeMap();
    level_x = 1;
    level_y = 0;
    levelManager = std::make_unique<LevelManager>(this, window.get(), textureManager.get());
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
  map = makeMap();
  levelManager->loadFirstLevel(worldId);

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
  levelManager = nullptr;
  b2DestroyWorld(worldId);
}

vector<vector<string>> WorldManager::makeMap() {
  vector<vector<string>> test_map{
      {"TestRoom1_big", "TestStartRoom", "TestRoom1", "TestRoom2"}};
  return test_map;
}

void WorldManager::changeLevel(direction dir) {
  switch (dir) { 
    case direction::UP:
      level_y--;
      break;
    case direction::DOWN:
      level_y++;
      break;
    case direction::LEFT:
      level_x--;
      break;
    case direction::RIGHT:
      level_x++;
      break;
    case direction::NONE:
      break;
  }
  levelManager->loadLevel(worldId ,map[level_y][level_x], dir);
}

void WorldManager::notifyTransition(direction dir) { changeLevel(dir); }