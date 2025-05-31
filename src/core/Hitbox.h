#pragma once

#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>

class Hitbox {
public:
	Hitbox(const b2WorldId& worldId, float pos_x, float pos_y, const b2Vec2& hitboxSize, float damage);
	void draw(sf::RenderWindow* window, sf::Color color) const;
	void move(float x, float y);
	void move(b2Vec2 mov);
	b2Vec2 getPosition();
	bool updateHitbox(long clock);
	bool isActive();
	void wake(b2Vec2& position, b2Rot& rotation);

private:
	b2BodyDef bodyDef;
	b2BodyId id;
	b2Polygon polygon;
	b2ShapeDef shapeDef;
	b2ShapeId shapeId;
	long clockTimeInit;
	bool activeHitbox;
};