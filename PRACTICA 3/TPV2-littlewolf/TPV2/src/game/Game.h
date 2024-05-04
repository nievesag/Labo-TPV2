// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include "../utils/Singleton.h"


class LittleWolf;
class Networking;

class Game : public Singleton<Game> {
	friend Singleton<Game>;
public:
	Game();
	virtual ~Game();
	void init();
	void start();

	LittleWolf& get_wolves() {
		return *little_wolf_;
	}

	Networking* get_networking();

private:

	LittleWolf *little_wolf_;
	Networking* networking_;

};

