#include "Entity.h"
#include <iostream>

Entity::Entity(const b2WorldId& worldId, float pos_x, float pos_y, int hp, b2Vec2& hurtboxSize, sf::Texture* texture,
	uint64_t categoryBitsHurtbox, uint64_t maskBitsHurtbox,
	uint64_t categoryBitsHitbox, uint64_t maskBitsHitbox,
	LevelMediator* levelMediator, bool renderDebugBoxes = false) :
	hurtbox{ std::make_unique<Hurtbox>(worldId, pos_x, pos_y, hurtboxSize, categoryBitsHurtbox, maskBitsHurtbox) },
	hitbox{ std::make_unique<Hitbox>(worldId, pos_x, pos_y, b2Vec2{hurtboxSize.x / 2, hurtboxSize.y * 4 / 3 }, 1, categoryBitsHitbox, maskBitsHitbox, levelMediator) },
	pos_x{ pos_x },
	pos_y{ pos_y },
	maxHp{ hp },
	hp{ hp },
	renderDebugBoxes{ renderDebugBoxes },
	texture_handler{ std::make_unique<TextureHandler>(*texture, std::vector<int>{8, 8}, 600) },
	levelMediator{ levelMediator }
{
	hurtbox->setType(b2_dynamicBody); //kinematic body a terme ?
	hurtbox->setLinearDamping(3);
	texture_handler->setScale(3);

	//float scaleX = 2 * hurtboxSize.x / texture->getSize().x;
	//float scaleY = 2 * hurtboxSize.y / texture->getSize().y;
	////sf::Vector2 size = sprite.getTexture().getSize();
	// float middleX = sprite_size / 2;
	// float middleY = sprite_size / 2;
	//sprite.scale({ scaleX, scaleY });
	//sprite.setOrigin({ middleX, middleY });
}

void Entity::attack(b2Vec2 direction, float damage) {
	b2Rot rot = b2ComputeRotationBetweenUnitVectors({1,0}, direction);
	b2Vec2 position = hurtbox->getPosition() + direction * sizeMultiplier;
	hitbox->wake(position, rot);
}



void Entity::renderEntity(sf::RenderWindow *window) {
	// sf::Vector2 size = sprite.getTexture().getSize();

	//A faire qu'une seule fois à l'initialisation de TextureHandler ?
	float middle = (float)texture_handler->getSize() / 2.f;
	sf::Sprite sprite = texture_handler->getSprite();
	sprite.scale({texture_handler->getScale(), texture_handler->getScale()});
	sprite.setOrigin({middle, middle});
	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	

	b2Vec2 pos = hurtbox->getPosition();
	sprite.setPosition(sf::Vector2f(pos.x, pos.y));
	window->draw(sprite);
	if (renderDebugBoxes) {
		hurtbox->draw(window, sf::Color::Red);
		hitbox->draw(window, sf::Color::Yellow);
	}
}

void Entity::move(float x, float y) {
	//hitbox.move(x, y);
}

void Entity::updateTempo() {
	//Choose action
	//TODO

	//Send action to texture_handler
	texture_handler->nextFrame();
}

void Entity::update(long clock) {
	//Update its hitbox
	hitbox->updateHitbox(clock);

	//Update the texture
	texture_handler->update(clock);
}

int32_t Entity::getShapeIndex() {
	return hurtbox->getShapeId().index1;
}

b2Vec2 Entity::getPosition() {
	return hurtbox->getPosition();
}

void Entity::updateDamage(int damage) {
	hp -= damage;
	if (hp <= 0) {
		levelMediator->notifyDeath(getShapeIndex());
	}
}

void Entity::destroy() {
	hitbox->destroy();
	hurtbox->destroy();
}