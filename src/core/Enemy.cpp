#include "Enemy.h"
#include <iostream>
#include "Constants.h"

b2Vec2 enemyHitbox{ 13, 13 };

const enum patternTypes {
	ACTION, WAIT
};

const std::vector<patternTypes> movementPattern = { WAIT, ACTION };

Enemy::Enemy(const b2WorldId& worldId, float pos_x, float pos_y,
	sf::Texture* texture, Graph* levelGraph,
	LevelMediator* levelMediator, bool renderDebugBoxes) :
	Entity(worldId, pos_x, pos_y, 3, enemyHitbox, texture,
		entityType::ENEMY_HURTBOX, entityType::WALL | entityType::PLAYER_HITBOX | entityType::PLAYER_HURTBOX,
		entityType::ENEMY_HITBOX, entityType::PLAYER_HURTBOX,
		levelMediator, renderDebugBoxes),
	levelGraph{ levelGraph },
	pathAdvancement{ 0 },
	pathLimit{ 0 }
{}

void Enemy::move(b2Vec2& target, float tempoMS) {
	auto pos = getPosition();
	auto bodyId = hurtbox->getBodyId();
	auto direction = b2Normalize(b2Sub(target, pos));
	float d = b2Distance(target, pos);
	float lambda = b2Body_GetLinearDamping(bodyId);
	float t = tempoMS / 1000;
	auto v = lambda * d / (1 - exp(-lambda * t));
	b2Body_SetLinearVelocity(bodyId, direction * v);
}

void Enemy::updateTempo() {}

b2Vec2 Enemy::getMoveCoords(b2Vec2& playerPos) {
	auto worldId = hurtbox->getWorldId();
	auto origin = hurtbox->getPosition();
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
			auto pos = getPosition();
			auto target = b2Normalize(b2Sub(playerPos, pos)) * sizeMultiplier + pos;
			pathLimit = 0;
			return target;
		}
		else if (entityFilter == entityType::WALL) {
			//Testing if half of the path has been traveled so far.
			if (pathAdvancement >= pathLimit) {
				auto my_pos = getPosition();
				path = levelGraph->getPath(my_pos, playerPos);
				pathDebug = levelGraph->getPathRender(path);
				pathAdvancement = 0;
				pathLimit = path.size() / 2;
			}
			pathAdvancement++;
			return path[pathAdvancement] * sizeMultiplier;
			
		}
		else {
			pathDebug = sf::VertexArray(sf::PrimitiveType::LineStrip, 2);
			pathDebug[0].position = sf::Vector2(origin.x, origin.y);
			pathDebug[1].position = sf::Vector2(raycastResult.point.x, raycastResult.point.y);
			pathDebug[0].color = sf::Color::Red;
			pathDebug[1].color = sf::Color::Red;
			std::cout << "Touching wrong entityType: " << entityFilter << std::endl;
			return getPosition();
		}
	}
}

void Enemy::updateTempo(b2Vec2 playerPos, float tempoMS) {
	auto target = getMoveCoords(playerPos);
	move(target, tempoMS);
}


void Enemy::renderEnemy(sf::RenderWindow* window) {
	Entity::renderEntity(window);
	window->draw(pathDebug);
}