#pragma once

#include "../ecs/Component.h"

struct Points : public ecs::Component // <- SON STRUCTS
{
	__CMPID_DECL__(ecs::cmp::POINTS)

	Points() : points_() {}
	virtual ~Points() {}

	// como todos los componentes son structs se puede acceder directamente a este num
	int points_;
};