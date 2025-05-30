#include "Hitbox.h"
#include <iostream>

Hitbox::Hitbox(const b2WorldId &worldId, float pos_x, float pos_y, const b2Vec2 &hitboxSize) {
    bodyDef = b2DefaultBodyDef();
    bodyDef.position = b2Vec2{ pos_x, pos_y };
    bodyDef.type = b2_dynamicBody;
    bodyDef.linearDamping= 3;
    id = b2CreateBody(worldId, &bodyDef);

    polygon = b2MakeBox(hitboxSize.x, hitboxSize.y);

    shapeDef = b2DefaultShapeDef();
    //shapeDef.isSensor = true;

    shapeDef.density = 1.0f;
    shapeDef.material.friction = 0.9f;
    shapeDef.enableSensorEvents = true;
    shapeId = b2CreatePolygonShape(id, &shapeDef, &polygon);
}

void Hitbox::Draw(sf::RenderWindow *window) const {
    b2Vec2 position = b2Body_GetPosition(id);

    sf::VertexArray lines(sf::PrimitiveType::LineStrip, polygon.count+1);
    for (int i = 1; i < polygon.count; i++) {
        b2Vec2 point = polygon.vertices[i] + position;
        lines[i].position = sf::Vector2f(point.x, point.y);
        lines[i].color = sf::Color::Red;
    }
    b2Vec2 point = polygon.vertices[0] + position;
    lines[0].position = sf::Vector2f(point.x, point.y);
    lines[0].color = sf::Color::Red;
    lines[polygon.count].position = sf::Vector2f(point.x, point.y);
    lines[polygon.count].color = sf::Color::Red;

    window->draw(lines);
}

void Hitbox::move(float x, float y) {
    b2Body_SetLinearVelocity(id, b2Vec2(x, y));
}