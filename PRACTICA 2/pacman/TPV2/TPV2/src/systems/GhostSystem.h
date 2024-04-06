#pragma once

#include "../ecs/System.h"

class GhostSystem : public ecs::System
{
public:
	__SYSID_DECL__(ecs::sys::GHOST)

	GhostSystem();
	virtual ~GhostSystem();
	void initSystem() override;
	void update() override;
	void recieve(const Message& m) override;

	void generateGhost();
	void killPacman();
	void moveGhosts();

private: 
	int ghostLimit_ = 10;
	int currentGhosts_ = 0;
	int followChance = 5;
};