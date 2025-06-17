#include "WorldManager.h"
#include <iostream>

using namespace std;
using namespace std::literals;

WorldManager::WorldManager() :
  worldDef{ b2DefaultWorldDef() },
  worldId{ b2CreateWorld(&worldDef) },
  window{ std::make_unique<sf::RenderWindow>(sf::VideoMode({ 1000, 700 }), "JinProject") },
  camera { std::make_unique<sf::View>() },
  textureManager{ std::make_unique<TextureManager>() },
  musicManager{ std::make_unique<MusicManager>() }
{
    window->setFramerateLimit(60);
    camera->setCenter({0, 0});
    camera->setSize({ 1000, 700 });
    window->setView(*camera);
    b2World_SetGravity(worldId, b2Vec2{ 0, 0 });
    level_x = 1;
    level_y = 0;
    floor = 0;
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
  loadLobby();
  
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

void WorldManager::makeMap() {
  worldMap = std::make_unique<struct worldMap>();

  struct floor floor;

  floor.first_room_x = 0;
  floor.first_room_y = 0;
  floor.music = RYTHMORTIS;
  floor.rooms = {
    {{"Lobby"}}
  };
  worldMap->floors.push_back(floor);

  floor.first_room_x = 1;
  floor.first_room_y = 0;
  floor.music = DISCO_DESCENT;
  floor.rooms = {
      {{"TestRoom1_big"}, {"TestStartRoom"}, {"TestRoom1"}, {"TestRoom2"} },
      {{""}, {""}, {"room_murs"}, {"TestRoom2"} }
  };
  worldMap->floors.push_back(floor);
}

void WorldManager::changeFloor(direction dir) {
  switch (dir) {
    case direction::STAGE_DOWN:
      floor++;
      break;
    case direction::STAGE_UP:
      floor--;
      break;
  }
  auto new_floor = worldMap->floors[floor];
  level_x = new_floor.first_room_x;
  level_y = new_floor.first_room_y;

  auto new_room = new_floor.rooms[level_y][level_x];
  levelManager->loadLevel(worldId, new_room.name, dir, new_room.cleared);
  levelManager->setTempo(musicManager->getMusicBPM(new_floor.music));
  levelManager->initTempo();
  musicManager->playMusic(new_floor.music);
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
  auto room = worldMap->floors[floor].rooms[level_y][level_x];
  levelManager->loadLevel(worldId, room.name, dir, room.cleared);
}

void WorldManager::loadLobby() {
  makeMap();

  floor = 0;
  auto lobby_floor = worldMap->floors[floor];
  level_x = lobby_floor.first_room_x;
  level_y = lobby_floor.first_room_y;

  auto lobby_room = lobby_floor.rooms[level_y][level_x].name;
  levelManager->loadLevel(worldId, lobby_room, direction::NONE, true);
  levelManager->setTempo(musicManager->getMusicBPM(lobby_floor.music));
  levelManager->initTempo();
  musicManager->playMusic(lobby_floor.music);
}

void WorldManager::notifyTransition(direction dir) { 
  worldMap->floors[floor].rooms[level_y][level_x].cleared = true;
  
  if (dir == direction::STAGE_UP || dir == direction::STAGE_DOWN) {
    changeFloor(dir);
  }
  else {
    changeLevel(dir);
  }
}