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
	void recieve(const Message& m) override;

private:
	uint32_t maxInmunityDuration = 10000;
	uint32_t timeOfActivation;

	ecs::entity_t pacman;
};