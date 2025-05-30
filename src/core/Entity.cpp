#include "Entity.h"

Entity::Entity(const b2WorldId &worldId, float pos_x, float pos_y, int hp, b2Vec2& hitboxSize, bool renderHitboxes=false) :
	hitbox{ Hitbox(worldId, pos_x, pos_y, hitboxSize) },
	pos_x{ pos_x },
	pos_y{ pos_y },
	maxHp{ hp },
	hp{ hp },
	renderHitboxes{ renderHitboxes }
{}

void Entity::renderEntity(sf::RenderWindow *window) {
	if (renderHitboxes) {
		hitbox.Draw(window);
	}
}

void Entity::move(float x, float y) {
	hitbox.move(x, y);
}