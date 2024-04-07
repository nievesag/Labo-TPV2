#pragma once

#include "GameState.h"
#include <SDL_rect.h>
class Texture;
class InputHandler;

class GameOverState : public GameState
{
public:
	GameOverState();
	virtual ~GameOverState();
	void enter() override;
	void leave() override;
	void update() override;

private:
	Texture* msg_;
	SDL_Rect dest_;
};