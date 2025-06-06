#pragma once

#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>
#include "LevelMediator.h"

class Hitbox {
public:
	//Constructor
	Hitbox(const b2WorldId& worldId, float pos_x, float pos_y, const b2Vec2& hitboxSize,
		float damage, uint64_t categoryBits, uint64_t maskBits,
		LevelMediator* levelMediator);
	
	void destroy();
	
	//Draws the hitbox on the window if debugDraw was activated
	void draw(sf::RenderWindow* window, sf::Color color);

	//Move function
	void move(float x, float y);
	//Move function
	void move(b2Vec2 mov);

	//GETTER: returns the hitbox's postion
	b2Vec2 getPosition();

	//GETTER: returns true if the hitbox is active
	bool isActive();

	//Updates the hitbox on every frame
	bool updateHitbox(long clock);
	//TODO: change the hitbox to die after a few frames only

	//Wakes the hitbox
	void wake(b2Vec2& position, b2Rot& rotation);

	bool entityTouched();

private:
	std::unique_ptr<b2BodyDef> bodyDef;
	std::unique_ptr<b2BodyId> id;
	std::unique_ptr<b2Polygon> polygon;
	std::unique_ptr <b2ShapeDef> shapeDef;
	std::unique_ptr <b2ShapeId> shapeId;
	long clockTimeInit;
	bool activeHitbox;
	LevelMediator* levelMediator;
	float damage;
};