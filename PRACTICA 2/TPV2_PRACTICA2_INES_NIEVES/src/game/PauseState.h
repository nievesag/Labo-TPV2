#pragma once

#include "GameState.h"
#include <SDL_rect.h>
class InputHandler;
class Texture;

class PauseState : public GameState
{
public:
	PauseState();
	virtual ~PauseState();
	void enter() override;
	void leave() override;
	void update() override;

private:
	Texture* msg_; // si lo pones con & te obliga a inicializarla en la constructora
	SDL_Rect dest_;
};