#include "Health.h"
#include <iostream>

Health::Health(int hp, int shapeIndex, LevelMediator* levelMediator, TextureManager* textureManager) :
	hp{ hp },
	maxHp{ hp },
	shapeIndex{ shapeIndex },
	shieldUp{ false },
	levelMediator{ levelMediator }
{
	auto texture = textureManager->getTexture(textureName::HEALTH_BAR);
	auto tsize = texture->getSize();
	sprite = std::make_unique<sf::Sprite>(*texture);
	sprite->setScale({ 1.4, 1.4 });
	sprite->setOrigin({healthBarWidth/2, healthBarHeight/2});
	sprite->setTextureRect(sf::IntRect({0, 0}, { healthBarWidth, healthBarHeight }));

}


void Health::renderHealthBar(sf::RenderWindow* window) {
	window->draw(*sprite);
}

void Health::updateHealthBar() {
	int healthSprite = (int) HEALTHBAR_NUMBER * (1 - ((float)hp / maxHp));
	auto health_bar_pos = sf::Vector2i{ 0, healthSprite * healthBarHeight};
	sprite->setTextureRect(sf::IntRect(health_bar_pos, { healthBarWidth, healthBarHeight}));
}

void Health::setHealthBarPosition(b2Vec2 position) {
	sprite->setPosition({ position.x, position.y });
}

void Health::updateDamage(int damage) {
	std::cout << "Damage !\n";
	if (shieldUp) {
		std::cout << "PARRY !\n";
		return;
	}
	hp -= damage;
	updateHealthBar();
	if (hp <= 0) {
		levelMediator->notifyDeath(shapeIndex);
	}
}

void Health::setShieldUp(bool isShieldUp) {
	shieldUp = isShieldUp;
}