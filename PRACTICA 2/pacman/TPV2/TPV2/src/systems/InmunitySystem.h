#pragma once

#include "../ecs/System.h"

class InmunitySystem : public ecs::System
{
public:
	__SYSID_DECL__(ecs::sys::INMUNITY)

	InmunitySystem();
	virtual ~InmunitySystem();
	void initSystem() override;
	void update() override;
};