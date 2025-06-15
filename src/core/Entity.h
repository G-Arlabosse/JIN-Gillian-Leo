#pragma once

#include "SFML/Graphics.hpp"
#include "Hurtbox.h"
#include "Hitbox.h"
#include "AnimationManager.h"
#include <vector>
#include <memory>
#include "Constants.h"
#include "LevelMediator.h"
#include "Health.h"

class Entity {
public:
	//Constructor
	Entity(const b2WorldId &worldId, float pos_x, float pos_y, int hp, b2Vec2& hitboxSize, 
		textureName textureName, TextureManager* textureManager,
		uint64_t categoryBitsHurtbox, uint64_t maskBitsHurtbox,
		uint64_t categoryBitsHitbox, uint64_t maskBitsHitbox,
		LevelMediator* levelMediator, bool renderDebugBoxes);
	
	//Destructor
	virtual ~Entity() = default;

	//Render the entity with its Texture Handler
	void renderEntity(sf::RenderWindow *window);

	/*
	-Entity ACTION lock must be free
	Attacks in the given direction
	Wakes the hitbox
	*/
	void attack(b2Vec2 direction, float damage);

	void shield();

	//GETTER: returns the position
	b2Vec2 getPosition();
	
	//GETTER: returns the index of the shapeId of the hurtbox
	int32_t getShapeIndex();

	//Updates the entity when the beat hits, called by LevelManager
	virtual void updateTempo();

	//Updates the entity each frame, called by LevelManager
	void update(long clock);

	//Updates the entity hit points, called by LevelManager
	void updateDamage(int damage);

	

protected:
	std::unique_ptr<Hurtbox> hurtbox;
	std::unique_ptr<Hitbox> hitbox;
	std::unique_ptr<Health> health;
	float pos_x;
	float pos_y;
	bool renderDebugBoxes;
  std::unique_ptr<AnimationManager> animation_manager;

	int patternState;

	long shieldClock;

	sf::Color hurtboxColor = sf::Color::Red;
};