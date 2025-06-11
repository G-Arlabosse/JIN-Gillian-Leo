#pragma once

#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>

class Hurtbox {
public:
	//Constructor
	Hurtbox(const b2WorldId &worldId, float pos_x, float pos_y, const b2Vec2 &hitboxSize, uint64_t categoryBits, uint64_t maskBits);

  ~Hurtbox() { b2DestroyBody(*id); };

	//Draws the hitbox on the window if debugDraw was activated
	void draw(sf::RenderWindow *window, sf::Color color) const;

	//Move function
	void move(float x, float y);
	//Move function
	void move(b2Vec2 mov);

	//SETTER: sets the bodyType of the box2d body
	void setType(b2BodyType bodyType);
	//TODO-Must be removed long term ?

	//SETTER: set the box2d linear dampling
	void setLinearDamping(float value);
	//TODO-Must be removed to make more controllable movement

	//GETTER: returns the hurtbox's position
	b2Vec2 getPosition();

	//GETTER: returns the worldId in which the hurbox lives
	b2WorldId getWorldId();
	b2BodyId getBodyId();
	b2ShapeId getShapeId();

private:
	std::unique_ptr<b2BodyDef> bodyDef;
	std::unique_ptr<b2BodyId> id;
	std::unique_ptr<b2Polygon> polygon;
	std::unique_ptr<b2ShapeDef> shapeDef;
	std::unique_ptr<b2ShapeId> shapeId;
};