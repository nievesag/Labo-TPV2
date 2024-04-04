#pragma once

#include "GameState.h"

class GameOverState : public GameState
{
public:
	GameOverState();
	virtual ~GameOverState();
	void enter() override;
	void leave() override;
	void update() override;
};