#pragma once

#include "Hitbox.h"
#include "Constants.h"


class LevelTransition {
 public:
  LevelTransition(const b2WorldId& worldId, float pos_x, float pos_y,
                  direction dir);
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
};
