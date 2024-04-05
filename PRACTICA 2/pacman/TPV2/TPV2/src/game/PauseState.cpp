#include "PauseState.h"

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "Game.h"

PauseState::PauseState()
{
	// guarda la textura correspondiente al menu de pausa
	msg_ = &sdlutils().msgs().at("paused");

	// hace el rect destino
	dest_ = build_sdlrect((sdlutils().width() - msg_->width()) / 2, (sdlutils().height() - msg_->height()) / 2, // pos x, y
						  msg_->width(), msg_->height()); // tamanio w, h
}

PauseState::~PauseState() {}

void PauseState::enter()
{
	// pausa el tiempo al entrar al estado
	sdlutils().virtualTimer().pause();
}

void PauseState::update()
{
	// si se pulsa cualquier tecla cambia a estado running
	if (ih().keyDownEvent()) {
		Game::instance()->setState(Game::RUNNING);
	}

	// muestra mensaje
	sdlutils().clearRenderer();
	msg_->render(dest_);
	sdlutils().presentRenderer();
}

void PauseState::leave()
{
	// reanuda el tiempo al salir del estado
	sdlutils().virtualTimer().resume();
}