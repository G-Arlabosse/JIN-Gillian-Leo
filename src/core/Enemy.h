#pragma once

#include "Entity.h"
#include "Player.h"
#include "Graph.h"

class Enemy : public Entity {
public:
	Enemy(const b2WorldId& worldId, float pos_x, float pos_y, sf::Texture* texture, Graph* levelGraph, bool renderDebugBoxes);
	void move(float x, float y) override;
	void updateTempo() override;
	void updateTempo(b2Vec2 playerPos);
	void renderEnemy(sf::RenderWindow* window);
	void getMoveCoords(b2Vec2& playerPos);
private:
	Graph* levelGraph;
	sf::VertexArray pathDebug;
	int patternState;
};