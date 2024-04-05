#include "NewGameState.h"

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "Game.h"

NewGameState::NewGameState()
{
	// guarda la textura correspondiente al menu de pausa
	msg_ = &sdlutils().msgs().at("newgame");

	// hace el rect destino
	dest_ = build_sdlrect((sdlutils().width() - msg_->width()) / 2, (sdlutils().height() - msg_->height()) / 2, // pos x, y
						  msg_->width(), msg_->height()); // tamanio w, h
}

NewGameState::~NewGameState() {}

void NewGameState::enter() {}

void NewGameState::update()
{
	// si se pulsa cualquier tecla cambia a estado new round
	if (ih().keyDownEvent()) {
		// faltaria reset lifes del pacman(?
		Game::instance()->setState(Game::NEWROUND);
	}

	// muestra mensaje
	sdlutils().clearRenderer();
	msg_->render(dest_);
	sdlutils().presentRenderer();
}

void NewGameState::leave() {}