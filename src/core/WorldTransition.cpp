#include "WorldTransition.h"

WorldTransition::WorldTransition(const b2WorldId& worldId, float pos_x,
    float pos_y, direction dir, TextureManager* textureManager): 
  dir{ dir }
{
  bodyDef = b2DefaultBodyDef();
  bodyDef.position = b2Vec2{ pos_x, pos_y };
  bodyDef.fixedRotation = true;
  bodyDef.linearVelocity = b2Vec2_zero;
  bodyDef.type = b2_staticBody;
  id = b2CreateBody(worldId, &bodyDef);

  switch (dir) {
  case direction::STAGE_UP:
    terminate();
    break;
  case direction::STAGE_DOWN:
    texture = textureManager->getTexture(textureName::WORLD_TRANSITION);
    polygon = b2MakeBox(sizeMultiplier/2, sizeMultiplier/2);
    sprite = std::make_unique<sf::Sprite>(*texture);
    sprite->setOrigin({ hitboxSize.x, hitboxSize.y });
    sprite->setPosition({ pos_x, pos_y });
    break;
  case direction::NONE:
    break;
  }
  if (!font.openFromFile("resources/font/pixelart.ttf"))
  {
    terminate();
  }
  // set the string to display
  text = std::make_unique<sf::Text>(font);
  std::string text_string = "Start game";
  auto size = sizeMultiplier/4;
  text->setString(text_string);
  text->setCharacterSize(size);
  sf::FloatRect rc = text->getLocalBounds();
  text->setOrigin(sf::Vector2f{ rc.size.x / 2, rc.size.y / 2 });

  text->setPosition(sf::Vector2f{ pos_x , pos_y - sizeMultiplier });

  shapeDef = b2DefaultShapeDef();
  shapeDef.isSensor = true;
  shapeDef.filter.categoryBits = entityType::LEVEL_TRANSITION;
  shapeDef.filter.maskBits = entityType::PLAYER_HURTBOX;

  shapeDef.enableSensorEvents = true;
  shapeId = b2CreatePolygonShape(id, &shapeDef, &polygon);
}

WorldTransition::~WorldTransition() { b2DestroyBody(id); }

bool WorldTransition::checkCollision() const {
  b2ShapeId overlaps[1]{ 0 };
  int shapeSensorCount = b2Shape_GetSensorOverlaps(shapeId, overlaps, 1);
  if (overlaps[0].index1 != 0 && b2Shape_IsValid(overlaps[0])) {
    return true;
  }
  if (shapeSensorCount > 0) {
    return true;
  }
  return false;
}

void WorldTransition::draw(sf::RenderWindow* window) const {
  b2Vec2 position = b2Body_GetPosition(id);
  b2Rot rotation = b2Body_GetRotation(id);

  sf::VertexArray lines(sf::PrimitiveType::LineStrip, polygon.count + 1);
  for (int i = 0; i < polygon.count; i++) {
    auto point = b2RotateVector(rotation, polygon.vertices[i]);
    lines[i].position = { position.x + point.x, position.y + point.y };
    lines[i].color = sf::Color(255, 0, 0);
  }
  lines[polygon.count].position = lines[0].position;
  lines[polygon.count].color = sf::Color(255, 0, 0);

  window->draw(lines);
  window->draw(*text);
  window->draw(*sprite);
}

direction WorldTransition::getDirection() { return dir; }

