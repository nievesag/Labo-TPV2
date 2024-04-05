#pragma once
#include "../ecs/Manager.h"

class GameState
{
public:
	GameState() {}

	virtual ~GameState() {}

	virtual void enter() = 0;
	virtual void leave() = 0;
	virtual void update() = 0;
	void setContext(ecs::Manager* manager);

protected:
	ecs::Manager* mngr_;

};