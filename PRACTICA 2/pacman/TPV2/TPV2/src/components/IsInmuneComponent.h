#pragma once

#include "../ecs/Component.h"

class IsInmuneComponent
{
	__CMPID_DECL__(ecs::cmp::INMUNE)

	IsInmuneComponent() : isInmune() {}
	virtual ~IsInmuneComponent() {}

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