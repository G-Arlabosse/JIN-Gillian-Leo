#include "Hitbox.h"
#include <iostream>

Hitbox::Hitbox(const b2WorldId &worldId, float pos_x, float pos_y, const b2Vec2 &hitboxSize) {
    bodyDef = b2DefaultBodyDef();
    bodyDef.position = b2Vec2{ pos_x, pos_y };
    bodyDef.fixedRotation = true;
    bodyDef.linearVelocity = b2Vec2_zero;
    id = b2CreateBody(worldId, &bodyDef);

    polygon = b2MakeBox(hitboxSize.x, hitboxSize.y);

    shapeDef = b2DefaultShapeDef();
    //shapeDef.isSensor = true;

    shapeDef.enableSensorEvents = true;
    shapeId = b2CreatePolygonShape(id, &shapeDef, &polygon);
}

void Hitbox::setType(b2BodyType bodyType) {
    b2Body_SetType(id, bodyType);
}

void Hitbox::setLinearDamping(float value) {
    b2Body_SetLinearDamping(id, value);
}

void Hitbox::draw(sf::RenderWindow *window, sf::Color color) const {
    b2Vec2 position = b2Body_GetPosition(id);

    sf::VertexArray lines(sf::PrimitiveType::LineStrip, polygon.count+1);
    for (int i = 1; i < polygon.count; i++) {
        b2Vec2 point = polygon.vertices[i] + position;
        lines[i].position = sf::Vector2f(point.x, point.y);
        lines[i].color = color;
    }
    b2Vec2 point = polygon.vertices[0] + position;
    lines[0].position = sf::Vector2f(point.x, point.y);
    lines[0].color = color;
    lines[polygon.count].position = sf::Vector2f(point.x, point.y);
    lines[polygon.count].color = color;

    window->draw(lines);
}

void Hitbox::move(float x, float y) {
    b2Vec2 velocity = b2Body_GetLinearVelocity(id);
    b2Body_SetLinearVelocity(id, b2Vec2(x, y));
}

b2Vec2 Hitbox::getPosition() {
    return b2Body_GetPosition(id);
}