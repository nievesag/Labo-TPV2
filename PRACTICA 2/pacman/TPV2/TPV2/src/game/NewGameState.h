#pragma once

#include "GameState.h"
#include <SDL_rect.h>
class Texture;
class InputHandler;

class NewGameState : public GameState
{
public:
	NewGameState();
	virtual ~NewGameState();
	void leave() override;
	void update() override;
	void enter() override;

private:
	Texture* msg_;
	SDL_Rect dest_;
};