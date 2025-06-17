#pragma once

#include "Hitbox.h"
#include "Wall.h"
#include "Constants.h"

class WorldTransition {
public:
  WorldTransition(const b2WorldId& worldId, float pos_x, float pos_y,
    direction dir, TextureManager* textureManager);
  ~WorldTransition();
  bool checkCollision() const;
  void draw(sf::RenderWindow* window) const;
  direction getDirection();

private:
  b2BodyDef bodyDef;
  b2BodyId id;
  b2Polygon polygon;
  b2ShapeDef shapeDef;
  b2ShapeId shapeId;
  direction dir;

  sf::Texture* texture;
  std::unique_ptr<sf::Sprite> sprite;
  sf::Font font;
  std::unique_ptr<sf::Text> text;
};