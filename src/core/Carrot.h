#pragma once

#include "Enemy.h"

class Carrot : public Enemy {
  Carrot(const b2WorldId& worldId, float pos_x, float pos_y,
         TextureManager* textureManager, float tempo,
         Graph* levelGraph, LevelMediator* levelMediator,
         bool renderDebugBoxes);
};