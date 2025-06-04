#pragma once

const int sizeMultiplier = 40;

const enum entityType {
    PLAYER_HURTBOX = 0x0001,
    PLAYER_HITBOX = 0x0002,
    ENEMY_HURTBOX = 0x0004,
    ENEMY_HITBOX = 0x0008,
    WALL = 0x0010,
    ENEMY_RAYCAST = 0x0020
};