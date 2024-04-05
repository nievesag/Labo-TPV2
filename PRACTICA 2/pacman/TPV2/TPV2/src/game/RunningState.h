#pragma once

#include "GameState.h"
#include "../systems/PacManSystem.h"



class RunningState : public GameState
{
public:
	RunningState();
	virtual ~RunningState();
	void leave() override;
	void update() override;
	void enter() override;

private:

	PacManSystem* pacmanSYS;
	
};