#include "Player.h"
#include "Constants.h"
#include "iostream"

b2Vec2 playerHitbox{ 13, 13 };

Player::Player(const b2WorldId& worldId, float pos_x, float pos_y, sf::Texture* texture, LevelMediator* levelMediator, bool renderDebugBoxes) :
	Entity(worldId, pos_x, pos_y, 3, playerHitbox, texture, 
		entityType::PLAYER_HURTBOX, entityType::WALL | entityType::ENEMY_HITBOX | entityType::ENEMY_HURTBOX | entityType::ENEMY_RAYCAST,
		entityType::PLAYER_HITBOX, entityType::ENEMY_HURTBOX,
		levelMediator, renderDebugBoxes),
	actionLocked{ false },
	tempoStreak{ 0 }
{
	//hurtbox->setLinearDamping(50);
}

void Player::attack(b2Vec2 direction, float damage) {
	Entity::attack(direction, damage);
}

void Player::move(b2Vec2& target) {
	if (!actionLocked) {
		b2Body_SetLinearVelocity(hurtbox->getBodyId(), target * 5);
	}
}

void Player::update(long clock, const sf::RenderWindow* window, bool inPlayerTempoWindow) {
	if (updateInput(window)) {
		if (inPlayerTempoWindow) { 
			tempoStreak++;
			hasMovedTempo = true;
		}
		else { tempoStreak = 0; }
		std::cout << tempoStreak << std::endl;
	}
	Entity::update(clock);
	if (actionLocked && clock > actionLockClock + 300) {
		unlockAction();
	}
}

b2Vec2 getMousePosition(const sf::RenderWindow* window) {
	auto posMouse = sf::Mouse::getPosition(*window);
	auto viewSize = window->getView().getSize();
	auto posCamera = window->getView().getCenter();
	return b2Vec2(posMouse.x - viewSize.x / 2 + posCamera.x, posMouse.y - viewSize.y / 2 + posCamera.y);
}

bool Player::updateInput(const sf::RenderWindow *window) {
	b2Vec2 offset = b2Vec2_zero;
	bool movement = false;
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		if (lockAction()) {
			auto mousePos = getMousePosition(window);
			auto direction = b2Normalize(b2Sub(mousePos, getPosition()));
			attack(direction, 0);
			return true;
		}
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
		if (lockAction()) {
			shield();
			return true;
		}
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
		auto target = b2Normalize(offset) * sizeMultiplier;
		move(target);
		tempoStreak = 0;
		return false;
	}
	return false;
}

void Player::notifyEndTempo()
{
	if (!hasMovedTempo && tempoStreak > 0) {
		tempoStreak = 0;
		std::cout << "Lost streak !\n";
	}
	hasMovedTempo = false;
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