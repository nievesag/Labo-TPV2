#pragma once


#include "../ecs/ecs.h"

using ecs::entity_t;

// FACHADA PARA CREAR MISILES
class MissileFacade
{
public:
	MissileFacade() = default; // default para que no pida crearlo en cpp
	virtual ~MissileFacade() = default; 
	virtual void create_missiles() = 0;
	virtual void remove_all_missiles() = 0;
};