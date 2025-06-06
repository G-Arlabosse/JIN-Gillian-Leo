#pragma once

//A global multiplier used to fit the textures size
const int sizeMultiplier = 40;

//All mask ids to fill box2d's filters
const enum entityType {
    PLAYER_HURTBOX = 0x0001,
    PLAYER_HITBOX = 0x0002,
    ENEMY_HURTBOX = 0x0004,
    ENEMY_HITBOX = 0x0008,
    WALL = 0x0010,
    ENEMY_RAYCAST = 0x0020
};