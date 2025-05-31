#pragma once

#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>

class Hurtbox {
public:
	Hurtbox(const b2WorldId &worldId, float pos_x, float pos_y, const b2Vec2 &hitboxSize);
	void draw(sf::RenderWindow *window, sf::Color color) const;
	void move(float x, float y);
	void move(b2Vec2 mov);
	void setType(b2BodyType bodyType);
	void setLinearDamping(float value);
	b2Vec2 getPosition();

private:
	b2BodyDef bodyDef;
	b2BodyId id;
	b2Polygon polygon;
	b2ShapeDef shapeDef;
	b2ShapeId shapeId;
};