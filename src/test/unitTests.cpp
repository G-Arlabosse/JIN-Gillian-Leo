#include <gtest/gtest.h>

#include "Graph.h"
#include "LevelManager.h"
#include "box2d/box2d.h"
#include "SFML/Graphics.hpp"

#include <iostream>

using namespace std;
TEST(Graphs, TestPath) {
  /*
  ---
  ##-
  ---
  ##-*/
  auto cons_dump = 
R"(Node 0 (0, 0) is empty and has 1 empty adjacent tiles : 
(0, 0) 
Node 1 (1, 0) is empty and has 2 empty adjacent tiles : 
(1, 0) (1, 0) 
Node 2 (2, 0) is empty and has 2 empty adjacent tiles : 
(2, 0) (2, 0) 
Node 5 (2, 1) is empty and has 3 empty adjacent tiles : 
(2, 1) (2, 1) (2, 1) 
Node 6 (0, 2) is empty and has 1 empty adjacent tiles : 
(0, 2) 
Node 7 (1, 2) is empty and has 2 empty adjacent tiles : 
(1, 2) (1, 2) 
Node 8 (2, 2) is empty and has 3 empty adjacent tiles : 
(2, 2) (2, 2) (2, 2) 
Node 11 (2, 3) is empty and has 1 empty adjacent tiles : 
(2, 3) 
)";

  Graph graph = Graph();
  graph.setWidth(3);
  graph.setHeight(4);
  graph.addNode(0, 0);
  graph.addNode(1, 0);
  graph.addNode(2, 0);
  graph.addNode(2, 1);
  graph.addNode(2, 2);
  graph.addNode(2, 3);
  graph.addNode(1, 2);
  graph.addNode(0, 2);
  graph.initGraphTransitions();

  auto dump = graph.dumpGraph();
  std::cout << dump;

  b2Vec2 start = { 0,0 };
  b2Vec2 goal = { 1,2 };
  auto path = graph.getPath(start, goal);
  EXPECT_EQ(dump, cons_dump);
  EXPECT_EQ(path.size(), 6);
}

TEST(Entities, TestAttack) {
  auto worldDef = b2DefaultWorldDef();
  auto worldId = b2CreateWorld(&worldDef);

  auto window = sf::RenderWindow();
  auto textureManager = TextureManager();
  auto levelManager = LevelManager(nullptr, &window, &textureManager);
  Player* player = levelManager.createPlayer(worldId, 0, 0, false);
  Enemy* enemy = levelManager.createEnemy(worldId, 40, 40, false);

  auto direction = b2Normalize({ 40,40 });
  player->attack(direction, 1);
  for (int i = 0; i < 10; i++) {
    levelManager.updateLevel(worldId);
  }

  EXPECT_EQ(enemy->getHealthPoints(), 2);
}

TEST(Entities, TestAttackMiss) {
  auto worldDef = b2DefaultWorldDef();
  auto worldId = b2CreateWorld(&worldDef);

  auto window = sf::RenderWindow();
  auto textureManager = TextureManager();
  auto levelManager = LevelManager(nullptr, &window, &textureManager);
  Player* player = levelManager.createPlayer(worldId, 0, 0, false);
  Enemy* enemy = levelManager.createEnemy(worldId, 40, 40, false);

  auto direction = b2Normalize({ -40,40 });
  player->attack(direction, 1);
  for (int i = 0; i < 10; i++) {
    levelManager.updateLevel(worldId);
  }

  EXPECT_EQ(enemy->getHealthPoints(), 3);
}

TEST(Entities, TestShield) {
  auto worldDef = b2DefaultWorldDef();
  auto worldId = b2CreateWorld(&worldDef);

  auto window = sf::RenderWindow();
  auto textureManager = TextureManager();
  auto levelManager = LevelManager(nullptr, &window, &textureManager);
  Player* player = levelManager.createPlayer(worldId, 0, 0, false);
  Enemy* enemy = levelManager.createEnemy(worldId, 40, 40, false);

  auto direction = b2Normalize({ 40,40 });
  enemy->shield();
  player->attack(direction, 1);
  for (int i = 0; i < 10; i++) {
    levelManager.updateLevel(worldId);
  }

  EXPECT_EQ(enemy->getHealthPoints(), 3);
}