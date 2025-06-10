#include "Hurtbox.h"
#include <iostream>

Hurtbox::Hurtbox(const b2WorldId &worldId, float pos_x, float pos_y, const b2Vec2 &hitboxSize, uint64_t categoryBits, uint64_t maskBits) {
    bodyDef = std::make_unique<b2BodyDef>(b2DefaultBodyDef());
    bodyDef->position = b2Vec2{ pos_x, pos_y };
    bodyDef->fixedRotation = true;
    bodyDef->linearVelocity = b2Vec2_zero;
    id = std::make_unique<b2BodyId>(b2CreateBody(worldId, bodyDef.get()));

    polygon = std::make_unique<b2Polygon>(b2MakeBox(hitboxSize.x, hitboxSize.y));

    shapeDef = std::make_unique<b2ShapeDef>(b2DefaultShapeDef());
    shapeDef->filter.categoryBits = categoryBits;
    shapeDef->filter.maskBits = maskBits;
    shapeDef->enableSensorEvents = true;

    shapeId = std::make_unique<b2ShapeId>(b2CreatePolygonShape(*id, shapeDef.get(), polygon.get()));
}

void Hurtbox::setType(b2BodyType bodyType) {
    b2Body_SetType(*id, bodyType);
}

void Hurtbox::setLinearDamping(float value) {
    b2Body_SetLinearDamping(*id, value);
}

void Hurtbox::draw(sf::RenderWindow *window, sf::Color color) const {
    b2Vec2 position = b2Body_GetPosition(*id);

    sf::VertexArray lines(sf::PrimitiveType::LineStrip, polygon->count+1);
    for (int i = 1; i < polygon->count; i++) {
        b2Vec2 point = polygon->vertices[i] + position;
        lines[i].position = sf::Vector2f(point.x, point.y);
        lines[i].color = color;
    }
    b2Vec2 point = polygon->vertices[0] + position;
    lines[0].position = sf::Vector2f(point.x, point.y);
    lines[0].color = color;
    lines[polygon->count].position = sf::Vector2f(point.x, point.y);
    lines[polygon->count].color = color;

    window->draw(lines);
}

void Hurtbox::move(float x, float y) {
    b2Body_SetLinearVelocity(*id, b2Vec2(x, y));
}
void Hurtbox::move(b2Vec2 mov) {
    b2Body_SetLinearVelocity(*id, mov);
}

b2Vec2 Hurtbox::getPosition() {
    return b2Body_GetPosition(*id);
}

b2WorldId Hurtbox::getWorldId() {
    return b2Body_GetWorld(*id);
}

b2ShapeId Hurtbox::getShapeId() {
    return *shapeId;
}

void Hurtbox::destroy() {
    b2Body_Disable(*id);
}