#pragma once

#include "GameState.h"

class RunningState : public GameState
{
public:
	RunningState();
	virtual ~RunningState();
	void leave() override;
	void update() override;
	void enter() override;
};