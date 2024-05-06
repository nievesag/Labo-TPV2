// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <SDL_stdinc.h>
#include "../utils/Singleton.h"


class LittleWolf;
class Networking;

class Game : public Singleton<Game> {
	friend Singleton<Game>;

public:
	Game();
	virtual ~Game();

	bool init(const char* host, Uint16 port);
	void start();

	LittleWolf* get_wolves();
	Networking* get_networking();

private:
	LittleWolf *little_wolf_;
	Networking* networking_;
};