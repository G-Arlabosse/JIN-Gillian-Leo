#include "Hitbox.h"
#include <iostream>

Hitbox::Hitbox(const b2WorldId& worldId, float pos_x, float pos_y, const b2Vec2& hitboxSize, 
        float damage, uint64_t categoryBits, uint64_t maskBits,
        LevelMediator* levelMediator):
    levelMediator{ levelMediator },
    damage{ 0 },
    activeHitbox{ false }
{
    bodyDef = std::make_unique<b2BodyDef>(b2DefaultBodyDef());
    bodyDef->position = b2Vec2{ pos_x, pos_y };
    bodyDef->fixedRotation = true;
    bodyDef->linearVelocity = b2Vec2_zero;
    bodyDef->type = b2_staticBody;
    id = std::make_unique<b2BodyId>(b2CreateBody(worldId, bodyDef.get()));

    clockTimeInit = std::clock();

    polygon = std::make_unique<b2Polygon>(b2MakeBox(hitboxSize.x, hitboxSize.y));

    shapeDef = std::make_unique<b2ShapeDef>(b2DefaultShapeDef());
    shapeDef->isSensor = true;
    shapeDef->filter.categoryBits = categoryBits;
    shapeDef->filter.maskBits = maskBits;

    shapeDef->enableSensorEvents = true;
    shapeId = std::make_unique<b2ShapeId>(b2CreatePolygonShape(*id, shapeDef.get(), polygon.get()));
}

bool Hitbox::entityTouched() {
    if (!activeHitbox) { return false; }
    bool touched = false;
    b2ShapeId overlaps[2]{ 0 };
    b2Shape_GetSensorOverlaps(*shapeId, overlaps, 2);
    for (const auto& overlap : overlaps) {
        auto overlapIndex = overlap.index1;
        if (overlapIndex != 0 && b2Shape_IsValid(overlap)) {
            levelMediator->notifyDamage(overlapIndex, damage, *id);
            touched = true;
        }
    }
    return touched;
}

void Hitbox::draw(sf::RenderWindow* window, sf::Color color) {
    if (!activeHitbox) { return; }
    
    if (entityTouched()) {
        color = sf::Color(0, 255, 0);
    }

    b2Vec2 position = b2Body_GetPosition(*id);
    b2Rot rotation = b2Body_GetRotation(*id);

    sf::VertexArray lines(sf::PrimitiveType::LineStrip, polygon->count + 1);
    for (int i = 0; i < polygon->count; i++) {
        auto point = b2RotateVector(rotation, polygon->vertices[i]);
        lines[i].position = {position.x + point.x, position.y + point.y};
        lines[i].color = color;
    }
    lines[polygon->count].position = lines[0].position;
    lines[polygon->count].color = color;

    window->draw(lines);
}

void Hitbox::wake(b2Vec2 &position, b2Rot &rotation) {
    clockTimeInit = std::clock();
    b2Body_SetTransform(*id, position, rotation);
    b2Body_SetAwake(*id, true);
    activeHitbox = true;
}

void Hitbox::move(float x, float y) {
    b2Body_SetLinearVelocity(*id, b2Vec2(x, y));
}
void Hitbox::move(b2Vec2 mov) {
    b2Body_SetLinearVelocity(*id, mov);
}

b2Vec2 Hitbox::getPosition() {
    return b2Body_GetPosition(*id);
}

/*
Updates the hitbox
Returns true if the hitbox must be detroyed
*/
bool Hitbox::updateHitbox(long clock) {
    if (clock > clockTimeInit) {
        b2Body_SetAwake(*id, false);
        activeHitbox = false;
        return true;
    }
    return false;
}

bool Hitbox::isActive() {
    return activeHitbox;
}

void Hitbox::destroy() {
    b2Body_Disable(*id);
}
