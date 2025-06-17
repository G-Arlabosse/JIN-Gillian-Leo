#include "Entity.h"
#include <iostream>

Entity::Entity(const b2WorldId& worldId, float pos_x, float pos_y, int hp, b2Vec2& hurtboxSize, 
	textureName textureName, TextureManager* textureManager, float tempo,
	uint64_t categoryBitsHurtbox, uint64_t maskBitsHurtbox,
	uint64_t categoryBitsHitbox, uint64_t maskBitsHitbox,
	LevelMediator* levelMediator, bool renderDebugBoxes = false): 
	hurtbox{std::make_unique<Hurtbox>(worldId, pos_x, pos_y, hurtboxSize,
                                        categoryBitsHurtbox, maskBitsHurtbox)},
  pos_x{pos_x},
  pos_y{pos_y},
  renderDebugBoxes{renderDebugBoxes},
  animation_manager{
		std::make_unique<AnimationManager>(textureName, textureManager, 1, tempo, 1.f, true)} {
  attack_animation_manager = std::make_unique<AnimationManager>(
      textureName::SLASH, textureManager, 1, tempo, 2, false);
  hitbox = std::make_unique<Hitbox>(worldId, std::pair<float, float>{pos_x, pos_y},
    std::pair<float, float>{0, 0}, b2Vec2{hurtboxSize.x, hurtboxSize.y * 4 / 3}, 1, tempo/2.f,
		std::move(attack_animation_manager), categoryBitsHitbox, maskBitsHitbox, levelMediator);
	health = std::make_unique<Health>(hp, hurtbox->getShapeId().index1, levelMediator, textureManager);
  hurtbox->setType(b2_dynamicBody);
  hurtbox->setLinearDamping(3);
}

void Entity::attack(b2Vec2 direction, float damage) {
	b2Rot rot = b2ComputeRotationBetweenUnitVectors({1,0}, direction);
	b2Vec2 position = hurtbox->getPosition() + direction * sizeMultiplier;
	hitbox->wake(position, rot);
  hitbox->reactivateAnimation();
}

void Entity::shield() {
	health->setShieldUp(true);
	shieldClock = std::clock();
	hurtboxColor = sf::Color::Blue;
}

void Entity::renderEntity(sf::RenderWindow *window) {  
	b2Vec2 pos = hurtbox->getPosition();
	animation_manager->draw(window, pos.x, pos.y);
	health->renderHealthBar(window);

	if (renderDebugBoxes) {
		hurtbox->draw(window, hurtboxColor);
		hitbox->draw(window, sf::Color::Yellow);
	}
}

void Entity::updateTempo() {
	//Choose action
	//TODO

	//Send action to texture_handler
	animation_manager->nextFrame();
}

void Entity::update(long clock) {
	if (clock > shieldClock + 200) {
		health->setShieldUp(false);
		hurtboxColor = sf::Color::Red;
	}

	auto hurtboxPos = hurtbox->getPosition();
	auto hurtboxSize = hurtbox->getSize();
	auto healthPos = b2Sub(hurtboxPos, b2Vec2{ 0, hurtboxSize.y/2 + sizeMultiplier/4 });
	health->setHealthBarPosition(healthPos);

	//Update its hitbox
	hitbox->updateHitbox(clock);

	//Update the texture
	animation_manager->update(clock);
}

int32_t Entity::getShapeIndex() {
	return hurtbox->getShapeId().index1;
}

b2Vec2 Entity::getPosition() {
	return hurtbox->getPosition();
}

void Entity::updateDamage(int damage) {
	health->updateDamage(damage);
}

int Entity::getHealthPoints() {
	return health->getHealthPoints();
}