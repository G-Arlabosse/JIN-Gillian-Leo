#pragma once

#include "Entity.h"
#include "Player.h"
#include "Graph.h"

const enum rayCastResult {
	MOVE_PLAYER, MOVE_WALL, ATTACK, ALERT
};

class Enemy : public Entity {
public:
	//Constructor
	Enemy(const b2WorldId& worldId, float pos_x, float pos_y, 
		textureName textureName, TextureManager* textureManager, Graph* levelGraph,
		LevelMediator* levelMediator, bool renderDebugBoxes);
	
	//Centralize later in Entity ?
	void move(b2Vec2& target, float tempoMS);

	/*Another updateTempo to get the playerPosition
	Should try to do it in another way ?*/
	void updateTempo(b2Vec2 playerPos, float tempoMS);

	//Temporary, just to draw the A* path
	void renderEnemy(sf::RenderWindow* window);

	/*
	When the MOVE_ACTION is called
	Returns the direction towards the enemy moves
	*/ 
	b2Vec2 getMoveCoords(b2Vec2& playerPos, rayCastResult result);
	rayCastResult getActionType(b2Vec2& playerPos);

private:
	Graph* levelGraph;
	sf::VertexArray pathDebug;
	std::vector<b2Vec2> path;
	int pathAdvancement;
	int pathLimit;
	int patternAdvancement;
};