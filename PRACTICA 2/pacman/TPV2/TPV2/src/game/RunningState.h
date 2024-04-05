#pragma once

#include "GameState.h"
#include "../systems/PacManSystem.h"
#include "../systems/CollisionsSystem.h"
#include "../systems/GhostSystem.h"
#include "../systems/FoodSystem.h"
#include "../systems/InmunitySystem.h"
#include "../systems/RenderSystem.h"

class RunningState : public GameState
{
public:
	RunningState(PacManSystem* pacmanSys, CollisionsSystem* collisionsSys, GhostSystem* ghostSys, 
				 FoodSystem* foodSys, InmunitySystem* inmunitySys, RenderSystem* RenderSys);
	virtual ~RunningState();
	void leave() override;
	void update() override;
	void enter() override;

private:

	PacManSystem* pacmanSys_;
	CollisionsSystem* collisionSys_;
	GhostSystem* ghostSys_;
	FoodSystem* foodSys_;
	InmunitySystem* inmunitySys_;
	RenderSystem* renderSys_;
};