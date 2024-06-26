// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <cstdint>
#include "../ecs/ecs.h"

using msgId_type = uint8_t;
enum msgId : msgId_type {
	_m_STAR_EATEN, 
	_m_CREATE_STARS, 
	_m_KILL_PACMAN,
	_m_EAT_FRUIT,
	_m_EAT_GHOST,
	_m_WIN_GAME,	
	_m_NEW_GAME,
	_m_PAUSE_GAME,
	_m_GAME_OVER,
	_m_ROUND_START,
	_m_ROUND_OVER,
	_m_PACMAN_FOOD_COLLISION,
	_m_PACMAN_GHOST_COLLISION,
	_m_IMMUNITY_START,
	_m_IMMUNITY_END
};

struct Message {

	msgId_type id;

	// if we use union we save memory, but then use only primitive
	// types, otherwise you will need to define constructors almost everywhere
	union {

		// _m_KILL_PACMAN
		struct {
			ecs::entity_t e;
		} kill_pacman_data;

		// _m_EAT_FRUIT
		struct {
			ecs::entity_t e;
		} eat_fruit_data;

		// _m_EAT_GHOST
		struct {
			ecs::entity_t e;
		} eat_ghost_data;
	};
};