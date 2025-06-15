#include "Wall.h"
#include "Constants.h"

const b2Vec2 wallSize = b2Vec2{ 1, 1 }*(sizeMultiplier/2);

Wall::Wall(const b2WorldId& worldId, float pos_x, float pos_y, TextureManager* textureManager, bool renderDebugBoxes) :
	hurtbox{ Hurtbox(worldId, pos_x, pos_y, wallSize,
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