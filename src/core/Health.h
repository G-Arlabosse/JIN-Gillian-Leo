#pragma once
#include "LevelMediator.h"
#include "SFML/Graphics.hpp"
#include "TextureManager.h"

#define HEALTHBAR_NUMBER 20

const int healthBarHeight = 8;
const int healthBarWidth = 32;

class Health {
public:
  Health(int hp, int shapeIndex, LevelMediator* levelMediator, TextureManager* textureManager);
  void renderHealthBar(sf::RenderWindow* window);
  void updateHealthBar();
  void setHealthBarPosition(b2Vec2 position);
  void updateDamage(int damage);
  void setShieldUp(bool isShieldUp);

private:
  int hp;
  int maxHp;
  int shapeIndex;
  bool shieldUp;
  LevelMediator* levelMediator;
  sf::Texture* texture;
  std::unique_ptr<sf::Sprite> sprite;
};