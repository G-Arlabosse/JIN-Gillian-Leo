#include "Wall.h"
#include "Constants.h"

Wall::Wall(const b2WorldId& worldId, float pos_x, float pos_y, bool renderDebugBoxes) :
	hurtbox{ Hurtbox(worldId, pos_x, pos_y, hitboxSize, 
		entityType::WALL, 
		entityType::PLAYER_HURTBOX | entityType::ENEMY_HURTBOX | entityType::ENEMY_RAYCAST) },
	renderDebugBoxes { renderDebugBoxes }
{
	hurtbox.setType(b2_staticBody);
}

void Wall::renderWall(sf::RenderWindow* window) const {
	if (renderDebugBoxes) {
		hurtbox.draw(window, sf::Color::Magenta);
	}	
}