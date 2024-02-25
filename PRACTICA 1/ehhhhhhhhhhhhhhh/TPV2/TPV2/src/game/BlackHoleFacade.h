#pragma once

#include "../ecs/ecs.h"

using ecs::entity_t;

class BlackHoleFacade {
public:
	BlackHoleFacade() { }
	virtual ~BlackHoleFacade() { }
	virtual void create_blackholes(int n) = 0;
	virtual void remove_all_blackholes() = 0;
	//virtual void split_blackholes(entity_t a) = 0;
};