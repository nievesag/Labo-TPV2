#include "NewRoundState.h"

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "Game.h"

NewRoundState::NewRoundState()
{
	// guarda la textura correspondiente al menu de pausa
	msg_ = &sdlutils().msgs().at("newround");

	// hace el rect destino
	dest_ = build_sdlrect((sdlutils().width() - msg_->width()) / 2, (sdlutils().height() - msg_->height()) / 2, // pos x, y
						  msg_->width(), msg_->height()); // tamanio w, h
}

NewRoundState::~NewRoundState() {}

void NewRoundState::enter()
{
}


void NewRoundState::update()
{
	// si se pulsa enter pasa a estado running
	if (ih().keyDownEvent() && ih().isKeyDown(SDL_SCANCODE_RETURN)) {
		// faltan cosas de logica creo(?
		Game::instance()->setState(Game::RUNNING);
	}

	sdlutils().clearRenderer();
	msg_->render(dest_);
	sdlutils().presentRenderer();
}

void NewRoundState::leave()
{
}