#include <iostream>

#include "LevelTransition.h"

LevelTransition::LevelTransition(const b2WorldId& worldId, float pos_x,
                                 float pos_y, direction dir, TextureManager* textureManager)
    : dir{dir} {
  bodyDef = b2DefaultBodyDef();
  bodyDef.position = b2Vec2{pos_x, pos_y};
  bodyDef.fixedRotation = true;
  bodyDef.linearVelocity = b2Vec2_zero;
  bodyDef.type = b2_staticBody;
  id = b2CreateBody(worldId, &bodyDef);

  texture = textureManager->getTexture(textureName::ROOM_TRANSITION);
  switch (dir) { 
    case direction::UP:
      polygon = b2MakeBox(hitboxSize.x*2, hitboxSize.y/2);
      sprite = std::make_unique<sf::Sprite>(*texture, sf::IntRect({96, 0}, {96, 48}));
      sprite->setPosition(
          {pos_x - hitboxSize.x * 2, pos_y - hitboxSize.y / 2.f});
      wall = std::make_unique<Wall>( worldId, pos_x, pos_y + hitboxSize.y,
          b2Vec2{hitboxSize.x * 2, hitboxSize.y / 2.f}, textureManager, true);
      break;
    case direction::DOWN:
      polygon = b2MakeBox(hitboxSize.x * 2, hitboxSize.y / 2);
      sprite =
          std::make_unique<sf::Sprite>(*texture, sf::IntRect({0, 0}, {96, 48}));
      sprite->setPosition(
          {pos_x - hitboxSize.x * 2, pos_y - hitboxSize.y * 3.f / 2.f});
      wall = std::make_unique<Wall>(worldId, pos_x, pos_y - hitboxSize.y,
          b2Vec2{hitboxSize.x * 2, hitboxSize.y / 2.f}, textureManager, true);
      break;
    case direction::LEFT:
      polygon = b2MakeBox(hitboxSize.x / 2, hitboxSize.y * 2);
      sprite = std::make_unique<sf::Sprite>(*texture,
                                            sf::IntRect({288, 0}, {48, 96}));
      sprite->setPosition(
          {pos_x - hitboxSize.x / 2.f, pos_y - hitboxSize.y * 2});
      wall = std::make_unique<Wall>(worldId, pos_x + hitboxSize.x, pos_y,
          b2Vec2{hitboxSize.x / 2.f, hitboxSize.y * 2}, textureManager, true);
      break;
    case direction::RIGHT:
      polygon = b2MakeBox(hitboxSize.x / 2, hitboxSize.y * 2);
      sprite = std::make_unique<sf::Sprite>(*texture,
                                            sf::IntRect({192, 0}, {48, 96}));
      sprite->setPosition(
          {pos_x - hitboxSize.x * 3.f / 2.f, pos_y - hitboxSize.y * 2});
      wall = std::make_unique<Wall>(worldId, pos_x - hitboxSize.x, pos_y,
          b2Vec2{hitboxSize.x / 2.f, hitboxSize.y * 2}, textureManager, true);
      break;
    case direction::NONE:
      break;
  }

  shapeDef = b2DefaultShapeDef();
  shapeDef.isSensor = true;
  shapeDef.filter.categoryBits = entityType::LEVEL_TRANSITION;
  shapeDef.filter.maskBits = entityType::PLAYER_HURTBOX;

  shapeDef.enableSensorEvents = true;
  shapeId = b2CreatePolygonShape(id, &shapeDef, &polygon);

  deactivated = false;
}

LevelTransition::~LevelTransition() { b2DestroyBody(id); }

bool LevelTransition::checkCollision() const {
  b2ShapeId overlaps[1]{0};
  int shapeSensorCount = b2Shape_GetSensorOverlaps(shapeId, overlaps, 1);
  if (overlaps[0].index1 != 0 && b2Shape_IsValid(overlaps[0])) {
    return true;
  }
  if (shapeSensorCount > 0) {
    return true;
  }
  return false;
}

void LevelTransition::deactivate() { 
  if (!deactivated) {
    wall = nullptr;
    deactivated = true;
  }
}

void LevelTransition::draw(sf::RenderWindow* window) const {
  b2Vec2 position = b2Body_GetPosition(id);
  b2Rot rotation = b2Body_GetRotation(id);

  sf::VertexArray lines(sf::PrimitiveType::LineStrip, polygon.count + 1);
  for (int i = 0; i < polygon.count; i++) {
    auto point = b2RotateVector(rotation, polygon.vertices[i]);
    lines[i].position = {position.x + point.x, position.y + point.y};
    lines[i].color = sf::Color(255,0,0);
  }
  lines[polygon.count].position = lines[0].position;
  lines[polygon.count].color = sf::Color(255, 0, 0);

  window->draw(lines);
  if (!deactivated)
    window->draw(*sprite);
}

direction LevelTransition::getDirection() { return dir; }

