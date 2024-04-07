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
	void moveGhosts();
	void resetGhosts();
	void destroyGhosts();
	void changeGhosts();
	void destroyGhost(ecs::entity_t ghost);

private: 
	int ghostLimit_; // maximo de fantasmas en pantalla
	int currentGhosts_; // fantasmas actuales en pantalla
	int followChance = 5; // probabilidad de que ese fantasma siga a pacman

	int lastSpawn; // tiempo de spawn del ultimo fantasma spawneado
	int ghostCD = 2000; // cooldown de generacion de fantasmas
};