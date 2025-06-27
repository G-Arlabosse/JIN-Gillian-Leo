#include "Corn.h"

Corn::Corn(const b2WorldId& worldId, float pos_x, float pos_y,
               TextureManager* textureManager, float tempo, Graph* levelGraph,
               LevelMediator* levelMediator, bool renderDebugBoxes)
    : Enemy(worldId, pos_x, pos_y, textureName::CORN, textureManager, tempo,
            levelGraph, levelMediator, renderDebugBoxes) {}