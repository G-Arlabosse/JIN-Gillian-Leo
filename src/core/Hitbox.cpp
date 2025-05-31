#include "Hitbox.h"
#include <iostream>

Hitbox::Hitbox(const b2WorldId& worldId, float pos_x, float pos_y, const b2Vec2& hitboxSize, float damage) {
    bodyDef = b2DefaultBodyDef();
    bodyDef.position = b2Vec2{ pos_x, pos_y };
    bodyDef.fixedRotation = true;
    bodyDef.linearVelocity = b2Vec2_zero;
    bodyDef.type = b2_staticBody;
    id = b2CreateBody(worldId, &bodyDef);

    clockTimeInit = std::clock();

    polygon = b2MakeBox(hitboxSize.x, hitboxSize.y);

    shapeDef = b2DefaultShapeDef();
    shapeDef.isSensor = true;

    shapeDef.enableSensorEvents = true;
    shapeId = b2CreatePolygonShape(id, &shapeDef, &polygon);
}

void Hitbox::draw(sf::RenderWindow* window, sf::Color color) const {
    if (!activeHitbox) { return; }

    b2Vec2 position = b2Body_GetPosition(id);
    b2Rot rotation = b2Body_GetRotation(id);

    sf::VertexArray lines(sf::PrimitiveType::LineStrip, polygon.count + 1);
    for (int i = 0; i < polygon.count; i++) {
        auto point = b2RotateVector(rotation, polygon.vertices[i]);
        lines[i].position = {position.x + point.x, position.y + point.y};
        lines[i].color = color;
    }
    lines[polygon.count].position = lines[0].position;
    lines[polygon.count].color = color;

    window->draw(lines);
}

void Hitbox::wake(b2Vec2 &position, b2Rot &rotation) {
    clockTimeInit = std::clock();
    b2Body_SetTransform(id, position, rotation);
    b2Body_SetAwake(id, true);
    activeHitbox = true;
}

void Hitbox::move(float x, float y) {
    b2Body_SetLinearVelocity(id, b2Vec2(x, y));
}
void Hitbox::move(b2Vec2 mov) {
    b2Body_SetLinearVelocity(id, mov);
}

b2Vec2 Hitbox::getPosition() {
    return b2Body_GetPosition(id);
}

/*
Updates the hitbox
Returns true if the hitbox must be detroyed
*/
bool Hitbox::updateHitbox(long clock) {
    if (clock > clockTimeInit + 300) {
        b2Body_SetAwake(id, false);
        activeHitbox = false;
        return true;
    }
    return false;
}

bool Hitbox::isActive() {
    return activeHitbox;
}