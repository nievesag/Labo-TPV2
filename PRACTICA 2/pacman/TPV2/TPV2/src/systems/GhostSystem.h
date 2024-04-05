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

private: 
	int ghostLimit_;
	int currentGhosts_;
};