#pragma once

#include "../ecs/Component.h"

class IsMiraculousComponent : public ecs::Component
{
	__CMPID_DECL__(ecs::cmp::INMUNE)

	IsMiraculousComponent() : isInmune() {}
	virtual ~IsMiraculousComponent() {}

	bool isInmune;

	bool getIsInmune()
	{
		return isInmune;
	}

	void setInmune(bool i)
	{
		isInmune = i;
	}
};