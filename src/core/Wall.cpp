#include "Wall.h"

Wall::Wall(const b2WorldId& worldId, float pos_x, float pos_y, const b2Vec2& hitboxSize, bool renderHitboxes) :
	hitbox { Hitbox(worldId, pos_x, pos_y, hitboxSize) },
	renderHitboxes { renderHitboxes }
{
	hitbox.setType(b2_staticBody);
}

void Wall::renderWall(sf::RenderWindow* window) const {
	if (renderHitboxes) {
		hitbox.draw(window, sf::Color::Magenta);
	}	
}