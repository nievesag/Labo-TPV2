#pragma once

#include "src/ecs/ecs.h"

using ecs::entity_t;

class MissileFacade
{
	MissileFacade() { }
	virtual ~MissileFacade() { }
	virtual void create_missile(int n) = 0;
	virtual void remove_all_missiles() = 0;
};