#pragma once

#include "box2d/types.h"

class LevelMediator {
public:
	virtual ~LevelMediator() = default;
	virtual void notifyDamage(int32_t hurtboxId, int damage) = 0;
	virtual void notifyDeath(int32_t hurtboxId) = 0;
};