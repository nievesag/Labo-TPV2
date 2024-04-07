#pragma once

#include "GameState.h"
#include <SDL_rect.h>
class Texture;
class InputHandler;

class NewRoundState : public GameState
{
public:
	NewRoundState();
	virtual ~NewRoundState();
	void enter() override;
	void leave() override;
	void update() override;

private:
	Texture* msg_;
	SDL_Rect dest_;
};