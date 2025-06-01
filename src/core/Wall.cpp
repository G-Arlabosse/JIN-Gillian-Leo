#include "Wall.h"
#include "Constants.h"

Wall::Wall(const b2WorldId& worldId, float pos_x, float pos_y, const b2Vec2& hitboxSize, bool renderDebugBoxes) :
	hurtbox{ Hurtbox(worldId, pos_x, pos_y, hitboxSize, entityType::WALL, entityType::PLAYER_HURTBOX | entityType::ENEMY_HURTBOX) },
	renderDebugBoxes { renderDebugBoxes }
{
	hurtbox.setType(b2_staticBody);
}

void Wall::renderWall(sf::RenderWindow* window) const {
	if (renderDebugBoxes) {
		hurtbox.draw(window, sf::Color::Magenta);
	}	
}