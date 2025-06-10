#include "Entity.h"
#include <iostream>

Entity::Entity(const b2WorldId& worldId, float pos_x, float pos_y, int hp,
               b2Vec2& hurtboxSize, sf::Texture* texture,
               uint64_t categoryBitsHurtbox, uint64_t maskBitsHurtbox,
               uint64_t categoryBitsHitbox, uint64_t maskBitsHitbox,
               LevelMediator* levelMediator, bool renderDebugBoxes = false)
    : hurtbox{std::make_unique<Hurtbox>(worldId, pos_x, pos_y, hurtboxSize,
                                        categoryBitsHurtbox, maskBitsHurtbox)},
      pos_x{pos_x},
      pos_y{pos_y},
      maxHp{hp},
      hp{hp},
      renderDebugBoxes{renderDebugBoxes},
      texture_handler{
          std::make_unique<TextureHandler>(*texture, std::vector<int>{8}, 3, 600, 1.f)},
      levelMediator{levelMediator} 
{
  hitbox = std::make_unique<Hitbox>(worldId, std::pair<float, float>{pos_x, pos_y},
      std::pair<float, float>{0, 0}, b2Vec2{hurtboxSize.x / 2, hurtboxSize.y * 4 / 3}, 1, 0,
      texture, categoryBitsHitbox, maskBitsHitbox, levelMediator);
  hurtbox->setType(b2_dynamicBody);
  hurtbox->setLinearDamping(3);
}

void Entity::attack(b2Vec2 direction, float damage) {
	b2Rot rot = b2ComputeRotationBetweenUnitVectors({1,0}, direction);
	b2Vec2 position = hurtbox->getPosition() + direction * sizeMultiplier;
	hitbox->wake(position, rot);
}



void Entity::renderEntity(sf::RenderWindow *window) {  
	b2Vec2 pos = hurtbox->getPosition();
	texture_handler->draw(window, pos.x, pos.y);

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