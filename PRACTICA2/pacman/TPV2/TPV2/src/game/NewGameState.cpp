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

void NewGameState::enter()
{
	// pausa el tiempo al entrar al estado
	sdlutils().virtualTimer().pause();
}

void NewGameState::update()  
{
	// si se pulsa cualquier tecla cambia a estado new round
	if (ih().keyDownEvent()) {

		// -- MENSAJE --
		// crea mensaje
		Message newGameMessage;

		// establece id

		newGameMessage.id = _m_NEW_GAME;

		// lo envia
		mngr_->send(newGameMessage, true);

		// cambia al estado
		Game::instance()->setState(Game::NEWROUND);
	}

	// muestra mensaje (menu)
	msg_->render(dest_);
}

void NewGameState::leave()
{
	// reanuda el tiempo al salir del estado
	sdlutils().virtualTimer().resume();
}