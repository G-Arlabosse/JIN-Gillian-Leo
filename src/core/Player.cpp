#include "Player.h"
#include "Constants.h"
#include "iostream"

b2Vec2 playerHitbox{ 13, 13 };

Player::Player(const b2WorldId& worldId, float pos_x, float pos_y, sf::Texture* texture, LevelMediator* levelMediator, bool renderDebugBoxes) :
	Entity(worldId, pos_x, pos_y, 3, playerHitbox, texture, 
		entityType::PLAYER_HURTBOX, entityType::WALL | entityType::ENEMY_HITBOX | entityType::ENEMY_HURTBOX | entityType::ENEMY_RAYCAST,
		entityType::PLAYER_HITBOX, entityType::ENEMY_HURTBOX,
		levelMediator, renderDebugBoxes),
	actionLocked{ false }
{
	hurtbox->setLinearDamping(50);
}

void Player::attack(b2Vec2 direction, float damage) {
	if (lockAction()) {
		Entity::attack(direction, damage);
	}
}

void Player::move(b2Vec2& target) {
	if (!actionLocked) {
		b2Body_SetLinearVelocity(hurtbox->getBodyId(), target * 5);
	}
}

void Player::updateTempo() {
	return;
}

void Player::update(long clock) {
	updateInput();
	Entity::update(clock);
	if (actionLocked && clock > actionLockClock + 300) {
		unlockAction();
	}
}

void Player::updateInput() {
	b2Vec2 offset = b2Vec2_zero;
	bool movement = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right)) {
		attack(b2Vec2(1, 0), 1);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left)) {
		attack(b2Vec2(-1, 0), 1);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up)) {
		attack(b2Vec2(0, -1), 1);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Down)) {
		attack(b2Vec2(0, 1), 1);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) {
		offset += {0, -1};
		movement = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
		offset += {-1, 0};
		movement = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
		offset += {0, 1};
		movement = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		offset += {1, 0};
		movement = true;
	}
	if (movement && !actionLocked) {
		std::cout << "Move\n";
		auto target = b2Normalize(offset) * sizeMultiplier;
		move(target);
	}

}

bool Player::lockAction() {
	if (!actionLocked) {
		actionLocked = true;
		actionLockClock = std::clock();
		return true;
	}
	return false;
}

void Player::unlockAction() {
	actionLocked = false;
}