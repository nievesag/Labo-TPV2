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
	// pausa el tiempo al entrar al estado
	sdlutils().virtualTimer().pause();
}

void NewRoundState::update()
{
	// si se pulsa enter pasa a estado running y envia mensaje
	if (ih().keyDownEvent() && ih().isKeyDown(SDL_SCANCODE_RETURN)) {

		Game::instance()->setState(Game::RUNNING);

		// -- MENSAJE --
		// crea mensaje
		Message newRoundMessage;

		// establece id
		newRoundMessage.id = _m_ROUND_START;

		// lo envia
		Game::instance()->getMngr()->send(newRoundMessage, true);

		// Cuando empieza una ronda, hay que resetear los contadores de tiempo
		// (el de generacion de fantasmas, el de frutas milagrosas)
	}

	// muestra mensaje
	msg_->render(dest_);
}

void NewRoundState::leave()
{
	// reanuda el tiempo al salir del estado
	sdlutils().virtualTimer().resume();
}