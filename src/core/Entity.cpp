#include "Entity.h"
#include <iostream>

Entity::Entity(const b2WorldId &worldId, float pos_x, float pos_y, int hp, b2Vec2& hitboxSize, sf::Texture* texture, bool renderHitboxes=false) :
	hitbox{ Hitbox(worldId, pos_x, pos_y, hitboxSize) },
	pos_x{ pos_x },
	pos_y{ pos_y },
	maxHp{ hp },
	hp{ hp },
	renderHitboxes{ renderHitboxes },
	sprite{ sf::Sprite(*texture) }
{
	hitbox.setType(b2_dynamicBody);
	hitbox.setLinearDamping(3);

	float scaleX = 5 * hitboxSize.x / texture->getSize().x;
	float scaleY = 5 * hitboxSize.y / texture->getSize().y;
	std::cout << scaleX << "\n";
	sf::Vector2 size = sprite.getTexture().getSize();
	float middleX = size.x / 2;
	float middleY = size.y / 2;
	sprite.scale({ scaleX, scaleY });
	sprite.setOrigin({ middleX, middleY });
}

void Entity::renderEntity(sf::RenderWindow *window) {
	b2Vec2 pos = hitbox.getPosition();
	sprite.setPosition(sf::Vector2f(pos.x, pos.y));

	window->draw(sprite);
	if (renderHitboxes) {
		hitbox.draw(window, sf::Color::Red);
	}
}

void Entity::move(float x, float y) {
	hitbox.move(x, y);
}