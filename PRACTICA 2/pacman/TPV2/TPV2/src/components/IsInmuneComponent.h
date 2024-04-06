#pragma once

#include "../ecs/Component.h"

struct IsInmuneComponent : public ecs::Component
{
	__CMPID_DECL__(ecs::cmp::INMUNE)

	IsInmuneComponent(bool isInmune_) : isInmune(isInmune_) {}
	virtual ~IsInmuneComponent() {}

	bool isInmune;

	void setInmune(bool i)
	{
		isInmune = i;
	}

	bool getIsInmune()
	{
		return isInmune;
	}
};