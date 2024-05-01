// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once


class LittleWolf;

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



private:

	LittleWolf *little_wolf_;

};

