#include "Entity.h"
#include <iostream>

Entity::Entity(const b2WorldId &worldId, float pos_x, float pos_y, int hp, b2Vec2& hurtboxSize, sf::Texture* texture, 
		uint64_t categoryBitsHurtbox, uint64_t maskBitsHurtbox,
		uint64_t categoryBitsHitbox, uint64_t maskBitsHitbox,
		bool renderDebugBoxes=false) :
	hurtbox{ Hurtbox(worldId, pos_x, pos_y, hurtboxSize, categoryBitsHurtbox, maskBitsHurtbox) },
	hitbox{ Hitbox(worldId, pos_x, pos_y, {hurtboxSize.x/2, hurtboxSize.y*4/3 }, 10, categoryBitsHitbox, maskBitsHitbox) },
	pos_x{ pos_x },
	pos_y{ pos_y },
	maxHp{ hp },
	hp{ hp },
	renderDebugBoxes{ renderDebugBoxes },
	sprite{ sf::Sprite(*texture) }
{
	hurtbox.setType(b2_dynamicBody); //kinematic body � terme
	hurtbox.setLinearDamping(3);

	float scaleX = 5 * hurtboxSize.x / texture->getSize().x;
	float scaleY = 5 * hurtboxSize.y / texture->getSize().y;
	sf::Vector2 size = sprite.getTexture().getSize();
	float middleX = size.x / 2;
	float middleY = size.y / 2;
	sprite.scale({ scaleX, scaleY });
	sprite.setOrigin({ middleX, middleY });
}

void Entity::attack(b2WorldId &worldId, b2Vec2 direction, float damage) {
	if (!hitbox.isActive()) {
		b2Rot rot = b2ComputeRotationBetweenUnitVectors({1,0}, direction);
		b2Vec2 position = hurtbox.getPosition() + direction * sizeMultiplier;
		hitbox.wake(position, rot);
	}
}

void Entity::renderEntity(sf::RenderWindow *window) {
	b2Vec2 pos = hurtbox.getPosition();
	sprite.setPosition(sf::Vector2f(pos.x, pos.y));

	window->draw(sprite);
	if (renderDebugBoxes) {
		hurtbox.draw(window, sf::Color::Red);
		hitbox.draw(window, sf::Color::Yellow);
	}
}

void Entity::update(long clock) {
	hitbox.updateHitbox(clock);
}

b2Vec2 Entity::getPosition() {
	return hurtbox.getPosition();
}