#pragma once

#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>
#include "LevelMediator.h"
#include "AnimationManager.h"

class Hitbox {
public:
	//Constructor
 Hitbox(const b2WorldId& worldId, std::pair<float, float> pos, std::pair<float, float> speed, const b2Vec2& hitboxSize,
	 float damage, long lifespan, textureName textureName, TextureManager* textureManager, uint64_t categoryBits, uint64_t maskBits,
		LevelMediator* levelMediator);
 ~Hitbox() { b2DestroyBody(*id); };
	
	//Draws the hitbox on the window if debugDraw was activated
	void draw(sf::RenderWindow* window, sf::Color color);

	//Move function
	void move(float x, float y);
	//Move function
  void move(b2Vec2 mov);
  // Move function
  void move();

	//GETTER: returns the hitbox's postion
  b2Vec2 getPosition();

  // GETTER: returns the hitbox's speed
  b2Vec2 getSpeed();

  // SETTER: sets the hitbox speed to {speed_x, speed_y}
  void setSpeed(float speed_x, float speed_y);

	//GETTER: returns true if the hitbox is active
	bool isActive();

	//Updates the hitbox on every frame
	bool updateHitbox(long clock);
	//TODO: change the hitbox to die after a few frames only

	//Wakes the hitbox
	void wake(b2Vec2& position, b2Rot& rotation);
	void sleep();

	bool entityTouched();

private:
	std::unique_ptr<b2BodyDef> bodyDef;
	std::unique_ptr<b2BodyId> id;
	std::unique_ptr<b2Polygon> polygon;
	std::unique_ptr <b2ShapeDef> shapeDef;
	std::unique_ptr <b2ShapeId> shapeId;
  b2Vec2 speed;
	long clockTimeInit;
  long lifespan;
	bool activeHitbox;
	LevelMediator* levelMediator;
	float damage;

	std::unique_ptr<AnimationManager> texture_handler;
};