#include "Entity.h"
#include <iostream>

int moveAmplitute = 200;
b2Vec2 movUp = { 0, -moveAmplitute };
b2Vec2 movRight = { moveAmplitute, 0 };
b2Vec2 movDown = { 0, moveAmplitute };
b2Vec2 movLeft = { -moveAmplitute, 0 };
b2Vec2 movWait = b2Vec2_zero;

std::vector<b2Vec2> movementPattern = { movRight, movWait, movLeft, movWait, 
movDown, movUp, movUp, movDown
};

Entity::Entity(const b2WorldId &worldId, float pos_x, float pos_y, int hp, b2Vec2& hurtboxSize, sf::Texture* texture, bool renderDebugBoxes=false) :
	hurtbox{ Hurtbox(worldId, pos_x, pos_y, hurtboxSize) },
	hitbox{ Hitbox(worldId, pos_x, pos_y, {hurtboxSize.x/2, hurtboxSize.y }, 10) },
	pos_x{ pos_x },
	pos_y{ pos_y },
	maxHp{ hp },
	hp{ hp },
	renderDebugBoxes{ renderDebugBoxes },
  texture_handler{TextureHandler(*texture, {8}, 600)}
{
	hurtbox.setType(b2_dynamicBody);
	hurtbox.setLinearDamping(3);
  texture_handler.setScale(3);
  sprite_scale = texture_handler.getScale();
  sprite_middle_x = (float)texture_handler.getSize_x() / 2.f;
  sprite_middle_y = (float)texture_handler.getSize_y() / 2.f;
}

void Entity::attack(b2WorldId &worldId, b2Vec2 direction, float damage) {
	if (!hitbox.isActive()) {
		b2Rot rot = b2ComputeRotationBetweenUnitVectors({1,0}, direction);
		b2Vec2 position = hurtbox.getPosition() + direction * 50;
		hitbox.wake(position, rot);
	}
}

void Entity::renderEntity(sf::RenderWindow *window) {
  sf::Sprite sprite = texture_handler.getSprite();
  sprite.scale({sprite_scale, sprite_scale});
  sprite.setOrigin({sprite_middle_x, sprite_middle_y});

	b2Vec2 pos = hurtbox.getPosition();
	sprite.setPosition(sf::Vector2f(pos.x, pos.y));

	window->draw(sprite);
	if (renderDebugBoxes) {
		hurtbox.draw(window, sf::Color::Red);
		hitbox.draw(window, sf::Color::Yellow);
	}
}

void Entity::move(float x, float y) {
	//hitbox.move(x, y);
}

void Entity::updateTempo() {
	patternState = (patternState + 1) % movementPattern.size();
	hurtbox.move(movementPattern[patternState]);
  texture_handler.nextFrame();
}

void Entity::update(long clock) {
	hitbox.updateHitbox(clock);
  texture_handler.update(clock);
}