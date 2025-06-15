#pragma once

#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>
#include "Hurtbox.h"

class TextureManager;

class Wall {
public:
	Wall(const b2WorldId& worldId, float pos_x, float pos_y, b2Vec2 size, TextureManager* textureManager, bool renderDebugBoxes);
	~Wall() = default;
	void renderWall(sf::RenderWindow* window) const;

private:
	std::unique_ptr<Hurtbox> hurtbox;
	bool renderDebugBoxes;
};