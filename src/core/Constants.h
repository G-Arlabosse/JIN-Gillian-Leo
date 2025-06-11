#pragma once

//A global multiplier used to fit the textures size
const int sizeMultiplier = 40;

const b2Vec2 hitboxSize{20, 20};

// Directions for level transitions
enum class direction { UP, DOWN, LEFT, RIGHT, NONE };

//All mask ids to fill box2d's filters
const enum entityType {
  PLAYER_HURTBOX = 0x0001,
  PLAYER_HITBOX = 0x0002,
  ENEMY_HURTBOX = 0x0004,
  ENEMY_HITBOX = 0x0008,
  WALL = 0x0010,
  ENEMY_RAYCAST = 0x0020,
  LEVEL_TRANSITION = 0x0040,
};