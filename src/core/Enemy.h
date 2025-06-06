#pragma once

#include "Entity.h"
#include "Player.h"
#include "Graph.h"

class Enemy : public Entity {
public:
	//Constructor
	Enemy(const b2WorldId& worldId, float pos_x, float pos_y, 
		sf::Texture* texture, Graph* levelGraph,
		LevelMediator* levelMediator, bool renderDebugBoxes);
	
	//Centralize later in Entity ?
	void move(float x, float y) override;

	void updateTempo() override;

	/*Another updateTempo to get the playerPosition
	Should try to do it in another way ?*/
	void updateTempo(b2Vec2 playerPos);

	//Temporary, just to draw the A* path
	void renderEnemy(sf::RenderWindow* window);

	/*
	When the MOVE_ACTION is called
	Returns the direction towards the enemy moves
	*/ 
	void getMoveCoords(b2Vec2& playerPos);
private:
	Graph* levelGraph;
	sf::VertexArray pathDebug;
};