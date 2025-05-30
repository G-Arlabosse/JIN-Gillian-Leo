#pragma once

#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>

class Hitbox {
public:
	Hitbox(const b2WorldId &worldId, float pos_x, float pos_y, const b2Vec2 &hitboxSize);
	void Draw(sf::RenderWindow *window) const;
	void move(float x, float y);

private:
	b2BodyDef bodyDef;
	b2BodyId id;
	b2Polygon polygon;
	b2ShapeDef shapeDef;
	b2ShapeId shapeId;
};