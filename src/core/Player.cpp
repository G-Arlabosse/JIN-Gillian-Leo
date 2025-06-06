#include "Player.h"
#include "Constants.h"

b2Vec2 playerHitbox{ 15, 15 };

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

void Player::move(float x, float y) {
	if (!actionLocked) {
		hurtbox->move(x, y);
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
		move(0, -200);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
		move(-200, 0);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
		move(0, 200);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		move(200, 0);
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