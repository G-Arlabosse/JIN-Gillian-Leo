#include "Enemy.h"
#include <iostream>
#include "Constants.h"

const int moveAmplitute = 200;
const b2Vec2 movUp = { 0, -moveAmplitute };
const b2Vec2 movRight = { moveAmplitute, 0 };
const b2Vec2 movDown = { 0, moveAmplitute };
const b2Vec2 movLeft = { -moveAmplitute, 0 };
const b2Vec2 movWait = b2Vec2_zero;

b2Vec2 enemyHitbox{ 15, 15 };

const std::vector<b2Vec2> movementPattern = { movRight, movWait, movLeft, movWait,
movDown, movUp, movUp, movDown
};

Enemy::Enemy(const b2WorldId& worldId, float pos_x, float pos_y, sf::Texture* texture, Graph* levelGraph, bool renderDebugBoxes) :
	Entity(worldId, pos_x, pos_y, 3, enemyHitbox, texture,
		entityType::ENEMY_HURTBOX, entityType::WALL | entityType::PLAYER_HITBOX | entityType::PLAYER_HURTBOX,
		entityType::ENEMY_HITBOX, entityType::PLAYER_HURTBOX,
		renderDebugBoxes),
	patternState{ 0 },
	levelGraph{ levelGraph }
{}

void Enemy::move(float x, float y)
{
	
}

void Enemy::updateTempo() {}

void Enemy::getMoveCoords(b2Vec2& playerPos) {
	auto worldId = hurtbox.getWorldId();
	auto origin = hurtbox.getPosition();
	auto translation = b2Sub(playerPos, origin);
	//Make it a field ?// // // // // // // // // // // // // // // // // // // // // // // //
	b2QueryFilter filter = b2DefaultQueryFilter();
	filter.categoryBits = entityType::ENEMY_RAYCAST;
	filter.maskBits = entityType::PLAYER_HURTBOX | entityType::WALL;
	// // // // // // // // // // // // // // // // // // // // // // // // // // // // // //
	b2RayResult raycastResult = b2World_CastRayClosest(worldId, origin, translation, filter);

	if (raycastResult.hit) {
		uint16_t entityFilter = b2Shape_GetFilter(raycastResult.shapeId).categoryBits;
		if (entityFilter == entityType::PLAYER_HURTBOX) {
			//move toward player
			pathDebug = sf::VertexArray(sf::PrimitiveType::LineStrip, 2);
			pathDebug[0].position = sf::Vector2(origin.x, origin.y);
			pathDebug[1].position = sf::Vector2(raycastResult.point.x, raycastResult.point.y);
			pathDebug[0].color = sf::Color::Cyan;
			pathDebug[1].color = sf::Color::Cyan;
		}
		else if (entityFilter == entityType::WALL) {
			//pathfinding
			auto my_pos = getPosition();
			auto path = levelGraph->getPath(my_pos, playerPos);
			pathDebug = levelGraph->getPathRender(path);
		}
		else {
			pathDebug = sf::VertexArray(sf::PrimitiveType::LineStrip, 2);
			pathDebug[0].position = sf::Vector2(origin.x, origin.y);
			pathDebug[1].position = sf::Vector2(raycastResult.point.x, raycastResult.point.y);
			pathDebug[0].color = sf::Color::Red;
			pathDebug[1].color = sf::Color::Red;
			std::cout << "Touching wrong entityType: " << entityFilter << std::endl;
		}
	}
}

void Enemy::updateTempo(b2Vec2 playerPos) {
	getMoveCoords(playerPos);
}


void Enemy::renderEnemy(sf::RenderWindow* window) {
	Entity::renderEntity(window);
	window->draw(pathDebug);
}